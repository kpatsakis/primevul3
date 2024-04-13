LRESULT OmniboxViewWin::OnMouseActivate(HWND window,
                                        UINT hit_test,
                                        UINT mouse_message) {
  LRESULT result = DefWindowProc(WM_MOUSEACTIVATE,
                                 reinterpret_cast<WPARAM>(window),
                                 MAKELPARAM(hit_test, mouse_message));
  if (!model_->has_focus() &&
      ((mouse_message == WM_LBUTTONDOWN || mouse_message == WM_RBUTTONDOWN)) &&
      (result == MA_ACTIVATE)) {
    DCHECK(!gaining_focus_.get());
    gaining_focus_.reset(new ScopedFreeze(this, GetTextObjectModel()));

    saved_selection_for_focus_change_.cpMin = -1;
  }
  return result;
}
