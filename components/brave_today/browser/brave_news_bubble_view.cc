// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "brave/components/brave_today/browser/brave_news_bubble_view.h"

#include <memory>

#include "base/bind.h"
#include "base/callback_helpers.h"
#include "base/strings/utf_string_conversions.h"
#include "brave/components/brave_today/browser/brave_news_tab_helper.h"
#include "brave/components/brave_today/common/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/web_contents.h"
#include "ui/accessibility/ax_enums.mojom-shared.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/base/ui_base_types.h"
#include "ui/gfx/geometry/insets.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/text_constants.h"
#include "ui/views/background.h"
#include "ui/views/bubble/bubble_border.h"
#include "ui/views/bubble/bubble_dialog_delegate_view.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/controls/button/md_text_button.h"
#include "ui/views/controls/label.h"
#include "ui/views/layout/flex_layout.h"
#include "ui/views/layout/flex_layout_types.h"
#include "ui/views/layout/layout_types.h"
#include "ui/views/style/typography.h"
#include "ui/views/view.h"
#include "ui/views/view_class_properties.h"

// static
void BraveNewsBubbleView::Show(views::View* anchor,
                               content::WebContents* contents) {
  views::BubbleDialogDelegateView::CreateBubble(
      std::make_unique<BraveNewsBubbleView>(anchor, contents))
      ->Show();
}

class BraveNewsFeedRow : public views::View {
 public:
  explicit BraveNewsFeedRow(BraveNewsTabHelper::FeedDetails details,
                            content::WebContents* contents)
      : contents_(contents) {
    DCHECK(contents_);
    tab_helper_ = BraveNewsTabHelper::FromWebContents(contents);

    auto* const layout =
        SetLayoutManager(std::make_unique<views::FlexLayout>());
    layout->SetOrientation(views::LayoutOrientation::kHorizontal);
    layout->SetMainAxisAlignment(views::LayoutAlignment::kStart);
    layout->SetCrossAxisAlignment(views::LayoutAlignment::kStretch);

    auto* title = AddChildView(
        std::make_unique<views::Label>(base::UTF8ToUTF16(details.title)));
    title->SetHorizontalAlignment(gfx::HorizontalAlignment::ALIGN_LEFT);
    title->SetProperty(
        views::kFlexBehaviorKey,
        views::FlexSpecification(views::MinimumFlexSizeRule::kScaleToMinimum,
                                 views::MaximumFlexSizeRule::kUnbounded));

    bool is_subscribed = tab_helper_->is_subscribed(details);
    auto* button = AddChildView(std::make_unique<views::MdTextButton>(
        base::BindRepeating(&BraveNewsFeedRow::OnPressed,
                            base::Unretained(this)),
        is_subscribed ? u"Unsubscribe" : u"Subscribe"));
    button->SetProminent(!is_subscribed);
  }

  ~BraveNewsFeedRow() override = default;

  void OnPressed() { LOG(ERROR) << "Pressed the button"; }

 private:
  raw_ptr<content::WebContents> contents_;
  raw_ptr<BraveNewsTabHelper> tab_helper_;
};

BraveNewsBubbleView::BraveNewsBubbleView(views::View* action_view,
                                         content::WebContents* contents)
    : views::BubbleDialogDelegateView(action_view,
                                      views::BubbleBorder::TOP_RIGHT,
                                      views::BubbleBorder::STANDARD_SHADOW),
      contents_(contents) {
  DCHECK(contents);

  SetButtons(ui::DIALOG_BUTTON_NONE);
  SetAccessibleRole(ax::mojom::Role::kDialog);
  set_adjust_if_offscreen(true);

  title_label_ = AddChildView(
      std::make_unique<views::Label>(u"Subscribe to this site via Brave News",
                                     views::style::CONTEXT_DIALOG_TITLE));

  views::FlexLayout* const layout =
      SetLayoutManager(std::make_unique<views::FlexLayout>());
  layout->SetOrientation(views::LayoutOrientation::kVertical);
  layout->SetMainAxisAlignment(views::LayoutAlignment::kStart);
  layout->SetCrossAxisAlignment(views::LayoutAlignment::kStretch);
  layout->SetCollapseMargins(true);

  auto* tab_helper = BraveNewsTabHelper::FromWebContents(contents);
  for (const auto& feed_item : tab_helper->available_feeds()) {
    auto* child = AddChildView(std::make_unique<BraveNewsFeedRow>(feed_item));
    child->SetProperty(views::kMarginsKey, gfx::Insets::TLBR(10, 0, 0, 0));
  }

  auto* dismiss_button = AddChildView(std::make_unique<views::MdTextButton>(
      base::BindRepeating(&BraveNewsBubbleView::DismissForever,
                          base::Unretained(this)),
      u"Hide and don't show this again"));
  dismiss_button->SetProperty(views::kMarginsKey,
                              gfx::Insets::TLBR(10, 0, 0, 0));
  dismiss_button->SetProperty(
      views::kFlexBehaviorKey,
      views::FlexSpecification(views::MinimumFlexSizeRule::kPreferred,
                               views::MaximumFlexSizeRule::kPreferred));
}

BraveNewsBubbleView::~BraveNewsBubbleView() = default;

void BraveNewsBubbleView::Update() {}

void BraveNewsBubbleView::DismissForever() {
  GetWidget()->Hide();
  auto* profile = Profile::FromBrowserContext(contents_->GetBrowserContext());
  profile->GetPrefs()->SetBoolean(
      brave_news::prefs::kBraveTodayActionViewHidden, true);
}

BEGIN_METADATA(BraveNewsBubbleView, views::BubbleDialogDelegateView)
END_METADATA
