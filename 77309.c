void OmniboxViewWin::OnKillFocus(HWND focus_wnd) {
  if (m_hWnd == focus_wnd) {
    SetMsgHandled(false);
    return;
  }

  model_->OnWillKillFocus(focus_wnd);

  ClosePopup();

  GetSelection(saved_selection_for_focus_change_);

  model_->OnKillFocus();

  ScopedFreeze freeze(this, GetTextObjectModel());
  DefWindowProc(WM_KILLFOCUS, reinterpret_cast<WPARAM>(focus_wnd), 0);

  SelectAll(true);
  PlaceCaretAt(0);
}
