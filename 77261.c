  virtual void GetAccessibleState(ui::AccessibleViewState* state) {
    views::NativeViewHost::GetAccessibleState(state);
    state->name = l10n_util::GetStringUTF16(IDS_ACCNAME_LOCATION);
    state->role = ui::AccessibilityTypes::ROLE_TEXT;
    state->value = omnibox_view_win_->GetText();
    state->state = ui::AccessibilityTypes::STATE_EDITABLE;
    size_t sel_start;
    size_t sel_end;
    omnibox_view_win_->GetSelectionBounds(&sel_start, &sel_end);
    state->selection_start = sel_start;
    state->selection_end = sel_end;
  }
