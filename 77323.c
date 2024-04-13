void OmniboxViewWin::OnSetFocus(HWND focus_wnd) {
  views::FocusManager* focus_manager = parent_view_->GetFocusManager();
  if (focus_manager) {
    focus_manager->SetFocusedView(parent_view_);
  } else {
    NOTREACHED();
  }

  model_->OnSetFocus(GetKeyState(VK_CONTROL) < 0);

  if (saved_selection_for_focus_change_.cpMin != -1) {
    SetSelectionRange(saved_selection_for_focus_change_);
    saved_selection_for_focus_change_.cpMin = -1;
  }

  SetMsgHandled(false);
}
