void BrowserTitlebar::ExecuteCommand(int command_id) {
  if (command_id == kShowWindowDecorationsCommand) {
    PrefService* prefs = browser_window_->browser()->profile()->GetPrefs();
    prefs->SetBoolean(prefs::kUseCustomChromeFrame,
                  !prefs->GetBoolean(prefs::kUseCustomChromeFrame));
    return;
  }

  browser_window_->browser()->ExecuteCommand(command_id);
}
