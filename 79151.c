void DevToolsWindow::CreateDevToolsBrowser() {
  std::string wp_key = GetDevToolsWindowPlacementPrefKey();
  PrefService* prefs = profile_->GetPrefs();
  const DictionaryValue* wp_pref = prefs->GetDictionary(wp_key.c_str());
  if (!wp_pref || wp_pref->empty()) {
    DictionaryPrefUpdate update(prefs, wp_key.c_str());
    DictionaryValue* defaults = update.Get();
    defaults->SetInteger("left", 100);
    defaults->SetInteger("top", 100);
    defaults->SetInteger("right", 740);
    defaults->SetInteger("bottom", 740);
    defaults->SetBoolean("maximized", false);
    defaults->SetBoolean("always_on_top", false);
  }

  browser_ = new Browser(Browser::CreateParams::CreateForDevTools(
      profile_,
      chrome::GetHostDesktopTypeForNativeView(
          web_contents_->GetView()->GetNativeView())));
  browser_->tab_strip_model()->AddWebContents(
      web_contents_, -1, content::PAGE_TRANSITION_AUTO_TOPLEVEL,
      TabStripModel::ADD_ACTIVE);
  GetRenderViewHost()->SyncRendererPrefs();
}
