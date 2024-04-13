void OmniboxViewWin::OnRButtonDblClk(UINT /*keys*/, const CPoint& /*point*/) {
  gaining_focus_.reset();  // See NOTE in OnMouseActivate().
  SetMsgHandled(false);
}
