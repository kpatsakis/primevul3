LRESULT OmniboxViewWin::OnImeComposition(UINT message,
                                         WPARAM wparam,
                                         LPARAM lparam) {
  if (ignore_ime_messages_) {
    return DefWindowProc(message, wparam, lparam);
  }

  ScopedFreeze freeze(this, GetTextObjectModel());
  OnBeforePossibleChange();
  LRESULT result = DefWindowProc(message, wparam, lparam);
  OnAfterPossibleChangeInternal((lparam & GCS_RESULTSTR) != 0);
  return result;
}
