void OmniboxViewWin::HandleKeystroke(UINT message,
                                     TCHAR key,
                                     UINT repeat_count,
                                     UINT flags) {
  ScopedFreeze freeze(this, GetTextObjectModel());
  OnBeforePossibleChange();

  if (key == ui::VKEY_HOME || key == ui::VKEY_END) {
    HKL layout = GetKeyboardLayout(0);
    DefWindowProc(message, key, MAKELPARAM(repeat_count, flags));
    ActivateKeyboardLayout(layout, KLF_REORDER);
  } else {
    DefWindowProc(message, key, MAKELPARAM(repeat_count, flags));
  }

  const int lang_options = SendMessage(EM_GETLANGOPTIONS, 0, 0);
  if (lang_options & IMF_AUTOKEYBOARD)
    SendMessage(EM_SETLANGOPTIONS, 0, lang_options & ~IMF_AUTOKEYBOARD);

  OnAfterPossibleChange();
}
