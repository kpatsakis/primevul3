void RedirectToNtpOrAppsPageIfNecessary(content::WebContents* contents,
                                        signin::Source source) {
  if (source != signin::SOURCE_SETTINGS &&
      source != signin::SOURCE_WEBSTORE_INSTALL) {
    RedirectToNtpOrAppsPage(contents, source);
  }
}
