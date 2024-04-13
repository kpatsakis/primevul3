bool TabsHighlightFunction::RunImpl() {
  DictionaryValue* info = NULL;
  EXTENSION_FUNCTION_VALIDATE(args_->GetDictionary(0, &info));

  int window_id = extension_misc::kCurrentWindowId;
  if (info->HasKey(keys::kWindowIdKey))
    EXTENSION_FUNCTION_VALIDATE(
        info->GetInteger(keys::kWindowIdKey, &window_id));

  Browser* browser = NULL;
  if (!GetBrowserFromWindowID(this, window_id, &browser))
    return false;

  TabStripModel* tabstrip = browser->tab_strip_model();
  ui::ListSelectionModel selection;
  int active_index = -1;

  Value* tab_value = NULL;
  EXTENSION_FUNCTION_VALIDATE(info->Get(keys::kTabsKey, &tab_value));

  std::vector<int> tab_indices;
  EXTENSION_FUNCTION_VALIDATE(extensions::ReadOneOrMoreIntegers(
      tab_value, &tab_indices));

  for (size_t i = 0; i < tab_indices.size(); ++i) {
    int index = tab_indices[i];

    if (!tabstrip->ContainsIndex(index)) {
      error_ = ErrorUtils::FormatErrorMessage(
          keys::kTabIndexNotFoundError, base::IntToString(index));
      return false;
    }

    if (active_index == -1)
      active_index = index;

    selection.AddIndexToSelection(index);
  }

  if (selection.empty()) {
    error_ = keys::kNoHighlightedTabError;
    return false;
  }

  selection.set_active(active_index);
  browser->tab_strip_model()->SetSelectionFromModel(selection);
  SetResult(
      browser->extension_window_controller()->CreateWindowValueWithTabs(
          GetExtension()));
  return true;
}
