Browser* GetBrowserInProfileWithId(Profile* profile,
                                   const int window_id,
                                   bool include_incognito,
                                   std::string* error_message) {
  Profile* incognito_profile =
      include_incognito && profile->HasOffTheRecordProfile() ?
          profile->GetOffTheRecordProfile() : NULL;
  for (chrome::BrowserIterator it; !it.done(); it.Next()) {
    Browser* browser = *it;
    if ((browser->profile() == profile ||
         browser->profile() == incognito_profile) &&
        ExtensionTabUtil::GetWindowId(browser) == window_id &&
        browser->window()) {
      return browser;
    }
  }

  if (error_message)
    *error_message = ErrorUtils::FormatErrorMessage(
        keys::kWindowNotFoundError, base::IntToString(window_id));

  return NULL;
}
