int OmniboxViewWin::GetIcon() const {
  return IsEditingOrEmpty() ?
      AutocompleteMatch::TypeToIcon(model_->CurrentTextType()) :
      toolbar_model_->GetIcon();
}
