void USBMountObserver::RemoveBrowserFromVector(const std::string& path) {
  BrowserIterator i = FindBrowserForPath(path);
  std::string mount_path;
  if (i != browsers_.end()) {
    registrar_.Remove(this,
                      NotificationType::BROWSER_CLOSED,
                      Source<Browser>(i->browser));
    mount_path = i->mount_path;
    browsers_.erase(i);
  }
  std::vector<Browser*> close_these;
  for (BrowserList::const_iterator it = BrowserList::begin();
       it != BrowserList::end(); ++it) {
    if ((*it)->type() == Browser::TYPE_POPUP) {
      if (*it && (*it)->GetTabContentsAt((*it)->selected_index())) {
        const GURL& url =
            (*it)->GetTabContentsAt((*it)->selected_index())->GetURL();
        if (url.SchemeIs(chrome::kChromeUIScheme) &&
            url.host() == chrome::kChromeUIFileBrowseHost &&
            url.ref().find(mount_path) != std::string::npos) {
          close_these.push_back(*it);
        }
      }
    }
  }
  for (size_t x = 0; x < close_these.size(); x++) {
    if (close_these[x]->window()) {
      close_these[x]->window()->Close();
    }
  }
}
