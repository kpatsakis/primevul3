bool TabsRemoveFunction::RunImpl() {
  Value* tab_value = NULL;
  EXTENSION_FUNCTION_VALIDATE(args_->Get(0, &tab_value));

  std::vector<int> tab_ids;
  EXTENSION_FUNCTION_VALIDATE(extensions::ReadOneOrMoreIntegers(
      tab_value, &tab_ids));

  for (size_t i = 0; i < tab_ids.size(); ++i) {
    Browser* browser = NULL;
    WebContents* contents = NULL;
    if (!GetTabById(tab_ids[i], profile(), include_incognito(),
                    &browser, NULL, &contents, NULL, &error_))
      return false;

    if (!browser->window()->IsTabStripEditable()) {
      error_ = keys::kTabStripNotEditableError;
      return false;
    }

    contents->Close();
  }
  return true;
}
