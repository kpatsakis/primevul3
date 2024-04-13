bool TabsDuplicateFunction::RunImpl() {
  int tab_id = -1;
  EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &tab_id));

  Browser* browser = NULL;
  TabStripModel* tab_strip = NULL;
  int tab_index = -1;
  if (!GetTabById(tab_id, profile(), include_incognito(),
                  &browser, &tab_strip, NULL, &tab_index, &error_)) {
    return false;
  }

  WebContents* new_contents = chrome::DuplicateTabAt(browser, tab_index);
  if (!has_callback())
    return true;

  TabStripModel* new_tab_strip = NULL;
  int new_tab_index = -1;
  ExtensionTabUtil::GetTabStripModel(new_contents,
                                     &new_tab_strip,
                                     &new_tab_index);
  if (!new_tab_strip || new_tab_index == -1) {
    return false;
  }

  SetResult(ExtensionTabUtil::CreateTabValue(
      new_contents,
      new_tab_strip, new_tab_index, GetExtension()));

  return true;
}
