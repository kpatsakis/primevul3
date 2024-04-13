bool OmniboxEditModel::CanPasteAndGo(const base::string16& text) const {
  if (!view_->command_updater()->IsCommandEnabled(IDC_OPEN_CURRENT_URL))
    return false;

  AutocompleteMatch match;
  ClassifyStringForPasteAndGo(text, &match, NULL);
  return match.destination_url.is_valid();
}
