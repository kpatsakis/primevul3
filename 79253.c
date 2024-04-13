bool TabsQueryFunction::RunImpl() {
  DictionaryValue* query = NULL;
  EXTENSION_FUNCTION_VALIDATE(args_->GetDictionary(0, &query));

  QueryArg active = ParseBoolQueryArg(query, keys::kActiveKey);
  QueryArg pinned = ParseBoolQueryArg(query, keys::kPinnedKey);
  QueryArg selected = ParseBoolQueryArg(query, keys::kHighlightedKey);
  QueryArg current_window = ParseBoolQueryArg(query, keys::kCurrentWindowKey);
  QueryArg focused_window =
      ParseBoolQueryArg(query, keys::kLastFocusedWindowKey);

  QueryArg loading = NOT_SET;
  if (query->HasKey(keys::kStatusKey)) {
    std::string status;
    EXTENSION_FUNCTION_VALIDATE(query->GetString(keys::kStatusKey, &status));
    loading = (status == keys::kStatusValueLoading) ? MATCH_TRUE : MATCH_FALSE;
  }

  URLPattern url_pattern(URLPattern::SCHEME_ALL, "<all_urls>");
  if (query->HasKey(keys::kUrlKey)) {
    std::string value;
    EXTENSION_FUNCTION_VALIDATE(query->GetString(keys::kUrlKey, &value));
    url_pattern = URLPattern(URLPattern::SCHEME_ALL, value);
  }

  std::string title;
  if (query->HasKey(keys::kTitleKey))
    EXTENSION_FUNCTION_VALIDATE(
        query->GetString(keys::kTitleKey, &title));

  int window_id = extension_misc::kUnknownWindowId;
  if (query->HasKey(keys::kWindowIdKey))
    EXTENSION_FUNCTION_VALIDATE(
        query->GetInteger(keys::kWindowIdKey, &window_id));

  int index = -1;
  if (query->HasKey(keys::kIndexKey))
    EXTENSION_FUNCTION_VALIDATE(
        query->GetInteger(keys::kIndexKey, &index));

  std::string window_type;
  if (query->HasKey(keys::kWindowTypeLongKey))
    EXTENSION_FUNCTION_VALIDATE(
        query->GetString(keys::kWindowTypeLongKey, &window_type));

  ListValue* result = new ListValue();
  for (chrome::BrowserIterator it; !it.done(); it.Next()) {
    Browser* browser = *it;
    if (!profile()->IsSameProfile(browser->profile()))
      continue;

    if (!browser->window())
      continue;

    if (!include_incognito() && profile() != browser->profile())
      continue;

    if (window_id >= 0 && window_id != ExtensionTabUtil::GetWindowId(browser))
      continue;

    if (window_id == extension_misc::kCurrentWindowId &&
        browser != GetCurrentBrowser())
      continue;

    if (!MatchesQueryArg(current_window, browser == GetCurrentBrowser()))
      continue;

    if (!MatchesQueryArg(focused_window, browser->window()->IsActive()))
      continue;

    if (!window_type.empty() &&
        window_type !=
        browser->extension_window_controller()->GetWindowTypeText())
      continue;

    TabStripModel* tab_strip = browser->tab_strip_model();
    for (int i = 0; i < tab_strip->count(); ++i) {
      const WebContents* web_contents = tab_strip->GetWebContentsAt(i);

      if (index > -1 && i != index)
        continue;

      if (!MatchesQueryArg(selected, tab_strip->IsTabSelected(i)))
        continue;

      if (!MatchesQueryArg(active, i == tab_strip->active_index()))
        continue;

      if (!MatchesQueryArg(pinned, tab_strip->IsTabPinned(i)))
        continue;

      if (!title.empty() && !MatchPattern(web_contents->GetTitle(),
                                          UTF8ToUTF16(title)))
        continue;

      if (!url_pattern.MatchesURL(web_contents->GetURL()))
        continue;

      if (!MatchesQueryArg(loading, web_contents->IsLoading()))
        continue;

      result->Append(ExtensionTabUtil::CreateTabValue(
          web_contents, tab_strip, i, GetExtension()));
    }
  }

  SetResult(result);
  return true;
}
