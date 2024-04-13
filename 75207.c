bool BrowserTitlebar::IsCommandEnabled(int command_id) const {
  if (command_id == kShowWindowDecorationsCommand)
    return true;

  return browser_window_->browser()->command_updater()->
      IsCommandEnabled(command_id);
}
