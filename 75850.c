NavigationEntry* NavigationController::CreateNavigationEntry(
    const GURL& url, const GURL& referrer, PageTransition::Type transition,
    Profile* profile) {
  GURL loaded_url(url);
  bool reverse_on_redirect = false;
  BrowserURLHandler::RewriteURLIfNecessary(
      &loaded_url, profile, &reverse_on_redirect);

  NavigationEntry* entry = new NavigationEntry(
      NULL,  // The site instance for tabs is sent on navigation
      -1,
      loaded_url,
      referrer,
      string16(),
      transition);
  entry->set_virtual_url(url);
  entry->set_user_typed_url(url);
  entry->set_update_virtual_url_with_url(reverse_on_redirect);
  return entry;
}
