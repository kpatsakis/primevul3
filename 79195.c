void DevToolsWindow::OpenInNewTab(const std::string& url) {
  content::OpenURLParams params(
      GURL(url), content::Referrer(), NEW_FOREGROUND_TAB,
      content::PAGE_TRANSITION_LINK, false);
  content::WebContents* inspected_web_contents = GetInspectedWebContents();
  if (inspected_web_contents) {
    inspected_web_contents->OpenURL(params);
  } else {
    chrome::HostDesktopType host_desktop_type;
    if (browser_) {
      host_desktop_type = browser_->host_desktop_type();
    } else {
      NOTREACHED();
      host_desktop_type = chrome::GetActiveDesktop();
    }

    const BrowserList* browser_list =
        BrowserList::GetInstance(host_desktop_type);
    for (BrowserList::const_iterator it = browser_list->begin();
         it != browser_list->end(); ++it) {
      if ((*it)->type() == Browser::TYPE_TABBED) {
        (*it)->OpenURL(params);
        break;
      }
    }
  }
}
