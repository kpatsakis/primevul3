bool GetTabById(int tab_id,
                Profile* profile,
                bool include_incognito,
                Browser** browser,
                TabStripModel** tab_strip,
                content::WebContents** contents,
                int* tab_index,
                std::string* error_message) {
  if (ExtensionTabUtil::GetTabById(tab_id, profile, include_incognito,
                                   browser, tab_strip, contents, tab_index))
    return true;

  if (error_message)
    *error_message = ErrorUtils::FormatErrorMessage(
        keys::kTabNotFoundError, base::IntToString(tab_id));

  return false;
}
