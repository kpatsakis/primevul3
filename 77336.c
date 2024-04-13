void OmniboxViewWin::SaveStateToTab(WebContents* tab) {
  DCHECK(tab);

  const AutocompleteEditModel::State model_state(
      model_->GetStateForTabSwitch());

  CHARRANGE selection;
  GetSelection(selection);
  GetStateAccessor()->SetProperty(tab->GetPropertyBag(),
      AutocompleteEditState(
          model_state,
          State(selection, saved_selection_for_focus_change_)));
}
