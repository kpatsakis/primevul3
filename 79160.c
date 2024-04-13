bool DevToolsWindow::FindInspectedBrowserAndTabIndex(Browser** browser,
                                                     int* tab) {
  content::WebContents* inspected_web_contents = GetInspectedWebContents();
  if (!inspected_web_contents)
    return false;

  for (chrome::BrowserIterator it; !it.done(); it.Next()) {
    int tab_index = it->tab_strip_model()->GetIndexOfWebContents(
        inspected_web_contents);
    if (tab_index != TabStripModel::kNoTab) {
      *browser = *it;
      *tab = tab_index;
      return true;
    }
  }
  return false;
}
