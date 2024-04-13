bool Extension::CanExecuteScriptOnPage(const GURL& page_url,
                                       const UserScript* script,
                                       std::string* error) const {
  if ((page_url.host() == GURL(Extension::ChromeStoreLaunchURL()).host()) &&
      !CanExecuteScriptEverywhere() &&
      !CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kAllowScriptingGallery)) {
    if (error)
      *error = errors::kCannotScriptGallery;
    return false;
  }

  if (page_url.SchemeIs(chrome::kChromeUIScheme) &&
      !CanExecuteScriptEverywhere())
    return false;

  if (script)
    return script->MatchesURL(page_url);

  for (size_t i = 0; i < host_permissions_.size(); ++i) {
    if (host_permissions_[i].MatchesURL(page_url))
      return true;
  }

  if (error) {
    *error = ExtensionErrorUtils::FormatErrorMessage(errors::kCannotAccessPage,
                                                     page_url.spec());
  }

  return false;
}
