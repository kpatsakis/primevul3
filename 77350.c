void OmniboxViewWin::Update(const WebContents* tab_for_state_restoring) {
  const bool visibly_changed_permanent_text =
      model_->UpdatePermanentText(toolbar_model_->GetText());

  const ToolbarModel::SecurityLevel security_level =
      toolbar_model_->GetSecurityLevel();
  const bool changed_security_level = (security_level != security_level_);

  if (!changed_security_level && !visibly_changed_permanent_text &&
      !tab_for_state_restoring)
    return;

  security_level_ = security_level;

  ScopedFreeze freeze(this, GetTextObjectModel());
  if (tab_for_state_restoring) {
    RevertAll();

    const AutocompleteEditState* state = GetStateAccessor()->GetProperty(
        tab_for_state_restoring->GetPropertyBag());
    if (state) {
      model_->RestoreState(state->model_state);

      SetSelectionRange(state->view_state.selection);
      saved_selection_for_focus_change_ =
          state->view_state.saved_selection_for_focus_change;
    }
  } else if (visibly_changed_permanent_text) {

    CHARRANGE sel;
    GetSelection(sel);
    const bool was_reversed = (sel.cpMin > sel.cpMax);
    const bool was_sel_all = (sel.cpMin != sel.cpMax) &&
      IsSelectAllForRange(sel);

    RevertAll();

    if (was_sel_all)
      SelectAll(was_reversed);
  } else if (changed_security_level) {
    EmphasizeURLComponents();
  }
}
