void OmniboxViewWin::OnKeyUp(TCHAR key,
                             UINT repeat_count,
                             UINT flags) {
  if (key == VK_CONTROL)
    model_->OnControlKeyChanged(false);

  if (!(flags & KF_ALTDOWN) &&
      (((key == VK_CONTROL) && (GetKeyState(VK_SHIFT) < 0)) ||
       ((key == VK_SHIFT) && (GetKeyState(VK_CONTROL) < 0)))) {
    ScopedFreeze freeze(this, GetTextObjectModel());

    string16 saved_text(GetText());
    CHARRANGE saved_sel;
    GetSelection(saved_sel);

    SetWindowText(L"");

    DefWindowProc(WM_KEYUP, key, MAKELPARAM(repeat_count, flags));

    SetWindowText(saved_text.c_str());
    SetSelectionRange(saved_sel);
    return;
  }

  SetMsgHandled(false);
}
