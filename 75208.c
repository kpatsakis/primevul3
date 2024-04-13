bool BrowserTitlebar::IsItemChecked(int command_id) const {
  if (command_id == kShowWindowDecorationsCommand) {
    PrefService* prefs = browser_window_->browser()->profile()->GetPrefs();
    return !prefs->GetBoolean(prefs::kUseCustomChromeFrame);
  }

  EncodingMenuController controller;
  if (controller.DoesCommandBelongToEncodingMenu(command_id)) {
    TabContents* tab_contents =
        browser_window_->browser()->GetSelectedTabContents();
    if (tab_contents) {
      return controller.IsItemChecked(browser_window_->browser()->profile(),
                                      tab_contents->encoding(),
                                      command_id);
    }
    return false;
  }

  NOTREACHED();
  return false;
}
