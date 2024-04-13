void OmniboxEditModel::InternalSetUserText(const base::string16& text) {
  user_text_ = text;
  just_deleted_text_ = false;
  inline_autocomplete_text_.clear();
  view_->OnInlineAutocompleteTextCleared();
}
