bool OmniboxViewWin::SkipDefaultKeyEventProcessing(
    const views::KeyEvent& event) {
  ui::KeyboardCode key = event.key_code();
  if (event.IsAltDown() && (key != ui::VKEY_HOME) &&
      views::NativeTextfieldWin::IsNumPadDigit(key,
          (event.flags() & ui::EF_EXTENDED) != 0))
    return true;

  switch (key) {
    case ui::VKEY_ESCAPE: {
      ScopedFreeze freeze(this, GetTextObjectModel());
      return model_->OnEscapeKeyPressed();
    }

    case ui::VKEY_RETURN:
      return true;

    case ui::VKEY_UP:
    case ui::VKEY_DOWN:
      return !event.IsAltDown();

    case ui::VKEY_DELETE:
    case ui::VKEY_INSERT:
      return !event.IsAltDown() && event.IsShiftDown() &&
          !event.IsControlDown();

    case ui::VKEY_X:
    case ui::VKEY_V:
      return !event.IsAltDown() && event.IsControlDown();

    case ui::VKEY_BACK:
    case ui::VKEY_OEM_PLUS:
      return true;

    default:
      return false;
  }
}
