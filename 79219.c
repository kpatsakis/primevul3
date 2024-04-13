void DevToolsWindow::WebContentsFocused(content::WebContents* contents) {
  Browser* inspected_browser = NULL;
  int inspected_tab_index = -1;
  if (IsDocked() && FindInspectedBrowserAndTabIndex(&inspected_browser,
                                                    &inspected_tab_index))
    inspected_browser->window()->WebContentsFocused(contents);
}
