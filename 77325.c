void OmniboxViewWin::OnSysChar(TCHAR ch,
                               UINT repeat_count,
                               UINT flags) {
  if (ch == VK_SPACE)
    SetMsgHandled(false);
}
