import styled from 'styled-components'

export const Box = styled.div`
  display: grid;
  grid-template-columns: 1fr 1fr 1fr;
  grid-gap: 6px;
`

export const Chip = styled.div`
  margin: 0 auto;
  width: auto;
  height: 48px;
  border-radius: 8px;
  border: 1px solid transparent;

  div {
    width: 100%;
    height: 100%;
    background: transparent;
    border-radius: 8px;
    overflow: hidden;
  }

  .is-light {
    background: #F6F6F9;
  }

  .is-dark {
    background: #17171F;
  }

  .is-sepia {
    background: #E9E0CA;
  }
`
