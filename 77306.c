void OmniboxViewWin::OnKeyDown(TCHAR key,
                               UINT repeat_count,
                               UINT flags) {
  delete_at_end_pressed_ = false;

  if (OnKeyDownAllModes(key, repeat_count, flags))
    return;

  if (popup_window_mode_) {
    DefWindowProc(GetCurrentMessage()->message, key, MAKELPARAM(repeat_count,
                                                                flags));
    return;
  }

  if (OnKeyDownOnlyWritable(key, repeat_count, flags))
    return;

  HandleKeystroke(GetCurrentMessage()->message, key, repeat_count, flags);
}
