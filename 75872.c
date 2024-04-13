void NavigationController::UpdateVirtualURLToURL(
    NavigationEntry* entry, const GURL& new_url) {
  GURL new_virtual_url(new_url);
  if (BrowserURLHandler::ReverseURLRewrite(
          &new_virtual_url, entry->virtual_url(), profile_)) {
    entry->set_virtual_url(new_virtual_url);
  }
}
