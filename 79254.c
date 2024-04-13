bool TabsCreateFunction::RunImpl() {
  DictionaryValue* args = NULL;
  EXTENSION_FUNCTION_VALIDATE(args_->GetDictionary(0, &args));

  int window_id = extension_misc::kCurrentWindowId;
  if (args->HasKey(keys::kWindowIdKey))
    EXTENSION_FUNCTION_VALIDATE(args->GetInteger(
        keys::kWindowIdKey, &window_id));

  Browser* browser = NULL;
  if (!GetBrowserFromWindowID(this, window_id, &browser))
    return false;

  if (!browser->is_type_tabbed() && browser->IsAttemptingToCloseBrowser())
    browser = chrome::FindTabbedBrowser(profile(), include_incognito(),
                                        browser->host_desktop_type());

  if (!browser || !browser->window())
    return false;

  WebContents* opener = NULL;
  if (args->HasKey(keys::kOpenerTabIdKey)) {
    int opener_id = -1;
    EXTENSION_FUNCTION_VALIDATE(args->GetInteger(
        keys::kOpenerTabIdKey, &opener_id));

    if (!ExtensionTabUtil::GetTabById(
            opener_id, profile(), include_incognito(),
            NULL, NULL, &opener, NULL))
      return false;
  }


  std::string url_string;
  GURL url;
  if (args->HasKey(keys::kUrlKey)) {
    EXTENSION_FUNCTION_VALIDATE(args->GetString(keys::kUrlKey,
                                                &url_string));
    url = ExtensionTabUtil::ResolvePossiblyRelativeURL(url_string,
                                                       GetExtension());
    if (!url.is_valid()) {
      error_ = ErrorUtils::FormatErrorMessage(keys::kInvalidUrlError,
                                                       url_string);
      return false;
    }
  }

  if (ExtensionTabUtil::IsCrashURL(url)) {
    error_ = keys::kNoCrashBrowserError;
    return false;
  }

  bool active = true;
  if (args->HasKey(keys::kSelectedKey))
    EXTENSION_FUNCTION_VALIDATE(args->GetBoolean(keys::kSelectedKey, &active));

  if (args->HasKey(keys::kActiveKey))
    EXTENSION_FUNCTION_VALIDATE(args->GetBoolean(keys::kActiveKey, &active));

  bool pinned = false;
  if (args->HasKey(keys::kPinnedKey))
    EXTENSION_FUNCTION_VALIDATE(args->GetBoolean(keys::kPinnedKey, &pinned));

  if (url.SchemeIs(extensions::kExtensionScheme) &&
      !extensions::IncognitoInfo::IsSplitMode(GetExtension()) &&
      browser->profile()->IsOffTheRecord()) {
    Profile* profile = browser->profile()->GetOriginalProfile();
    chrome::HostDesktopType desktop_type = browser->host_desktop_type();

    browser = chrome::FindTabbedBrowser(profile, false, desktop_type);
    if (!browser) {
      browser = new Browser(Browser::CreateParams(Browser::TYPE_TABBED,
                                                  profile, desktop_type));
      browser->window()->Show();
    }
  }

  int index = -1;
  if (args->HasKey(keys::kIndexKey))
    EXTENSION_FUNCTION_VALIDATE(args->GetInteger(keys::kIndexKey, &index));

  TabStripModel* tab_strip = browser->tab_strip_model();

  index = std::min(std::max(index, -1), tab_strip->count());

  int add_types = active ? TabStripModel::ADD_ACTIVE : TabStripModel::ADD_NONE;
  add_types |= TabStripModel::ADD_FORCE_INDEX;
  if (pinned)
    add_types |= TabStripModel::ADD_PINNED;
  chrome::NavigateParams params(browser, url, content::PAGE_TRANSITION_LINK);
  params.disposition = active ? NEW_FOREGROUND_TAB : NEW_BACKGROUND_TAB;
  params.tabstrip_index = index;
  params.tabstrip_add_types = add_types;
  chrome::Navigate(&params);

  tab_strip = params.browser->tab_strip_model();
  int new_index = tab_strip->GetIndexOfWebContents(params.target_contents);
  if (opener)
    tab_strip->SetOpenerOfWebContentsAt(new_index, opener);

  if (active) {
    params.target_contents->GetDelegate()->ActivateContents(
        params.target_contents);
  }

  if (has_callback()) {
    SetResult(ExtensionTabUtil::CreateTabValue(
        params.target_contents,
        tab_strip, new_index, GetExtension()));
  }

  return true;
}
