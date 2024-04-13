void OmniboxViewWin::OnWindowPosChanging(WINDOWPOS* window_pos) {
  if (force_hidden_)
    window_pos->flags &= ~SWP_SHOWWINDOW;
  SetMsgHandled(true);
}
