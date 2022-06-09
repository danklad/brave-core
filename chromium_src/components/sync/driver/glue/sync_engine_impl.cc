/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "src/components/sync/driver/glue/sync_engine_impl.cc"

namespace syncer {

void SyncEngineImpl::PermanentlyDeleteAccount(base::OnceClosure callback) {
  DCHECK(backend_);
  sync_task_runner_->PostTask(
      FROM_HERE,
      base::BindOnce(&SyncEngineBackend::PermanentlyDeleteAccount, backend_,
                     BindToCurrentSequence(std::move(callback))));
}

}  // namespace syncer
