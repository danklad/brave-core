import * as React from 'react'

import * as S from './style'

const themeOptions = [
  {
    name: 'light',
    svgIcon: '<svg width="88" height="50" fill="none" xmlns="http://www.w3.org/2000/svg"><path fill="#fff" d="M11 0h66v50H11z"/><rect opacity=".6" x="22" y="13" width="44" height="4" rx="2" fill="#212529"/><rect opacity=".6" x="22" y="22" width="40" height="4" rx="2" fill="#495057"/><rect opacity=".6" x="22" y="31" width="36" height="4" rx="2" fill="#495057"/></svg>'
  },
  {
    name: 'sepia',
    svgIcon: '<svg width="88" height="50" fill="none" xmlns="http://www.w3.org/2000/svg"><path fill="#F2EBD9" d="M11 0h66v50H11z"/><rect opacity=".6" x="22" y="13" width="44" height="4" rx="2" fill="#331E05"/><rect opacity=".6" x="22" y="22" width="40" height="4" rx="2" fill="#5B4635"/><rect opacity=".6" x="22" y="31" width="36" height="4" rx="2" fill="#5B4635"/></svg>'
  },
  {
    name: 'dark',
    svgIcon: '<svg width="88" height="50" fill="none" xmlns="http://www.w3.org/2000/svg"><path fill="#1E2029" d="M11 0h66v50H11z"/><rect opacity=".6" x="22" y="13" width="44" height="4" rx="2" fill="#F0F2FF"/><rect opacity=".6" x="22" y="22" width="40" height="4" rx="2" fill="#C2C4CF"/><rect opacity=".6" x="22" y="31" width="36" height="4" rx="2" fill="#C2C4CF"/></svg>'
  }
]

function ThemeControl () {
  return (
    <S.Box>
      {themeOptions.map(entry => {
        return (
          <S.Chip>
            <div
              className={`is-${entry.name}`}
              dangerouslySetInnerHTML={{ __html: entry.svgIcon }}
            />
          </S.Chip>
        )
      })}
    </S.Box>
  )
}

export default ThemeControl
