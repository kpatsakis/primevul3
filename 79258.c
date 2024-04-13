bool TabsUpdateFunction::RunImpl() {
  DictionaryValue* update_props = NULL;
  EXTENSION_FUNCTION_VALIDATE(args_->GetDictionary(1, &update_props));

  Value* tab_value = NULL;
  if (HasOptionalArgument(0)) {
    EXTENSION_FUNCTION_VALIDATE(args_->Get(0, &tab_value));
  }

  int tab_id = -1;
  WebContents* contents = NULL;
  if (tab_value == NULL || tab_value->IsType(Value::TYPE_NULL)) {
    Browser* browser = GetCurrentBrowser();
    if (!browser) {
      error_ = keys::kNoCurrentWindowError;
      return false;
    }
    contents = browser->tab_strip_model()->GetActiveWebContents();
    if (!contents) {
      error_ = keys::kNoSelectedTabError;
      return false;
    }
    tab_id = SessionID::IdForTab(contents);
  } else {
    EXTENSION_FUNCTION_VALIDATE(tab_value->GetAsInteger(&tab_id));
  }

  int tab_index = -1;
  TabStripModel* tab_strip = NULL;
  if (!GetTabById(tab_id, profile(), include_incognito(),
                  NULL, &tab_strip, &contents, &tab_index, &error_)) {
    return false;
  }

  web_contents_ = contents;


  bool is_async = false;
  if (!UpdateURLIfPresent(update_props, tab_id, &is_async))
    return false;

  bool active = false;
  if (update_props->HasKey(keys::kSelectedKey))
    EXTENSION_FUNCTION_VALIDATE(update_props->GetBoolean(
        keys::kSelectedKey, &active));

  if (update_props->HasKey(keys::kActiveKey))
    EXTENSION_FUNCTION_VALIDATE(update_props->GetBoolean(
        keys::kActiveKey, &active));

  if (active) {
    if (tab_strip->active_index() != tab_index) {
      tab_strip->ActivateTabAt(tab_index, false);
      DCHECK_EQ(contents, tab_strip->GetActiveWebContents());
    }
    web_contents_->GetDelegate()->ActivateContents(web_contents_);
  }

  if (update_props->HasKey(keys::kHighlightedKey)) {
    bool highlighted = false;
    EXTENSION_FUNCTION_VALIDATE(update_props->GetBoolean(
        keys::kHighlightedKey, &highlighted));
    if (highlighted != tab_strip->IsTabSelected(tab_index))
      tab_strip->ToggleSelectionAt(tab_index);
  }

  if (update_props->HasKey(keys::kPinnedKey)) {
    bool pinned = false;
    EXTENSION_FUNCTION_VALIDATE(update_props->GetBoolean(
        keys::kPinnedKey, &pinned));
    tab_strip->SetTabPinned(tab_index, pinned);

    tab_index = tab_strip->GetIndexOfWebContents(contents);
  }

  if (update_props->HasKey(keys::kOpenerTabIdKey)) {
    int opener_id = -1;
    EXTENSION_FUNCTION_VALIDATE(update_props->GetInteger(
        keys::kOpenerTabIdKey, &opener_id));

    WebContents* opener_contents = NULL;
    if (!ExtensionTabUtil::GetTabById(
            opener_id, profile(), include_incognito(),
            NULL, NULL, &opener_contents, NULL))
      return false;

    tab_strip->SetOpenerOfWebContentsAt(tab_index, opener_contents);
  }

  if (!is_async) {
    PopulateResult();
    SendResponse(true);
  }
  return true;
}
