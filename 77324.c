LRESULT OmniboxViewWin::OnSetText(const wchar_t* text) {
  AutoReset<bool> auto_reset_ignore_ime_messages(&ignore_ime_messages_, true);
  return DefWindowProc(WM_SETTEXT, 0, reinterpret_cast<LPARAM>(text));
}
