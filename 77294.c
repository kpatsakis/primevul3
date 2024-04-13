void OmniboxViewWin::OnChar(TCHAR ch, UINT repeat_count, UINT flags) {
  if (ch == VK_RETURN && (flags & KF_ALTDOWN))
    return;

  if (ch == VK_ESCAPE && !(flags & KF_ALTDOWN))
    return;

  if (ch == VK_TAB) {
    return;
  }

  HandleKeystroke(GetCurrentMessage()->message, ch, repeat_count, flags);
}
