void USBMountObserver::OpenFileBrowse(const std::string& url,
                                      const std::string& device_path,
                                      bool small) {
  Browser* browser;
  Profile* profile;
  profile = BrowserList::GetLastActive()->profile();
  if (small) {
    browser = FileBrowseUI::OpenPopup(profile,
                                      url,
                                      FileBrowseUI::kSmallPopupWidth,
                                      FileBrowseUI::kSmallPopupHeight);
  } else {
    browser = FileBrowseUI::OpenPopup(profile,
                                      url,
                                      FileBrowseUI::kPopupWidth,
                                      FileBrowseUI::kPopupHeight);
  }
  registrar_.Add(this,
                 NotificationType::BROWSER_CLOSED,
                 Source<Browser>(browser));
  BrowserIterator iter = FindBrowserForPath(device_path);
  if (iter == browsers_.end()) {
    BrowserWithPath new_browser;
    new_browser.browser = browser;
    new_browser.device_path = device_path;
    browsers_.push_back(new_browser);
  } else {
    iter->browser = browser;
  }
}
