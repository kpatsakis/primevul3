std::string DevToolsWindow::GetDevToolsWindowPlacementPrefKey() {
  return std::string(prefs::kBrowserWindowPlacement) + "_" +
      std::string(kDevToolsApp);
}
