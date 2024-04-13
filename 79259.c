bool TabsMoveFunction::RunImpl() {
  Value* tab_value = NULL;
  EXTENSION_FUNCTION_VALIDATE(args_->Get(0, &tab_value));

  std::vector<int> tab_ids;
  EXTENSION_FUNCTION_VALIDATE(extensions::ReadOneOrMoreIntegers(
      tab_value, &tab_ids));

  DictionaryValue* update_props = NULL;
  int new_index;
  EXTENSION_FUNCTION_VALIDATE(args_->GetDictionary(1, &update_props));
  EXTENSION_FUNCTION_VALIDATE(update_props->GetInteger(keys::kIndexKey,
      &new_index));

  ListValue tab_values;
  for (size_t i = 0; i < tab_ids.size(); ++i) {
    Browser* source_browser = NULL;
    TabStripModel* source_tab_strip = NULL;
    WebContents* contents = NULL;
    int tab_index = -1;
    if (!GetTabById(tab_ids[i], profile(), include_incognito(),
                    &source_browser, &source_tab_strip, &contents,
                    &tab_index, &error_))
      return false;

    if (!source_browser->window()->IsTabStripEditable()) {
      error_ = keys::kTabStripNotEditableError;
      return false;
    }

    new_index += i;

    if (update_props->HasKey(keys::kWindowIdKey)) {
      Browser* target_browser = NULL;
      int window_id = extension_misc::kUnknownWindowId;
      EXTENSION_FUNCTION_VALIDATE(update_props->GetInteger(
          keys::kWindowIdKey, &window_id));

      if (!GetBrowserFromWindowID(this, window_id, &target_browser))
        return false;

      if (!target_browser->window()->IsTabStripEditable()) {
        error_ = keys::kTabStripNotEditableError;
        return false;
      }

      if (!target_browser->is_type_tabbed()) {
        error_ = keys::kCanOnlyMoveTabsWithinNormalWindowsError;
        return false;
      }

      if (target_browser->profile() != source_browser->profile()) {
        error_ = keys::kCanOnlyMoveTabsWithinSameProfileError;
        return false;
      }

      if (ExtensionTabUtil::GetWindowId(target_browser) !=
          ExtensionTabUtil::GetWindowId(source_browser)) {
        TabStripModel* target_tab_strip = target_browser->tab_strip_model();
        WebContents* web_contents =
            source_tab_strip->DetachWebContentsAt(tab_index);
        if (!web_contents) {
          error_ = ErrorUtils::FormatErrorMessage(
              keys::kTabNotFoundError, base::IntToString(tab_ids[i]));
          return false;
        }

        if (new_index > target_tab_strip->count() || new_index < 0)
          new_index = target_tab_strip->count();

        target_tab_strip->InsertWebContentsAt(
            new_index, web_contents, TabStripModel::ADD_NONE);

        if (has_callback()) {
          tab_values.Append(ExtensionTabUtil::CreateTabValue(
              web_contents,
              target_tab_strip,
              new_index,
              GetExtension()));
        }

        continue;
      }
    }

    if (new_index >= source_tab_strip->count() || new_index < 0)
      new_index = source_tab_strip->count() - 1;

    if (new_index != tab_index)
      source_tab_strip->MoveWebContentsAt(tab_index, new_index, false);

    if (has_callback()) {
      tab_values.Append(ExtensionTabUtil::CreateTabValue(
          contents, source_tab_strip, new_index, GetExtension()));
    }
  }

  if (!has_callback())
    return true;

  if (tab_ids.size() > 1) {
    SetResult(tab_values.DeepCopy());
  } else if (tab_ids.size() == 1) {
    Value* value = NULL;
    CHECK(tab_values.Get(0, &value));
    SetResult(value->DeepCopy());
  }
  return true;
}
