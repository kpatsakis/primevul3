bool ExecuteCodeInTabFunction::Init() {
  if (details_.get())
    return true;

  int tab_id = -1;
  if (args_->GetInteger(0, &tab_id))
    EXTENSION_FUNCTION_VALIDATE(tab_id >= 0);

  DictionaryValue* details_value = NULL;
  if (!args_->GetDictionary(1, &details_value))
    return false;
  scoped_ptr<InjectDetails> details(new InjectDetails());
  if (!InjectDetails::Populate(*details_value, details.get()))
    return false;

  if (tab_id == -1) {
    Browser* browser = GetCurrentBrowser();
    if (!browser)
      return false;
    content::WebContents* web_contents = NULL;
    if (!ExtensionTabUtil::GetDefaultTab(browser, &web_contents, &tab_id))
      return false;
  }

  execute_tab_id_ = tab_id;
  details_ = details.Pass();
  return true;
}
