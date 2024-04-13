bool DevToolsWindow::IsInspectedBrowserPopup() {
  Browser* browser = NULL;
  int tab;
  return FindInspectedBrowserAndTabIndex(&browser, &tab) &&
      browser->is_type_popup();
}
