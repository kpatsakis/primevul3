void DevToolsWindow::SetDockSide(const std::string& side) {
  DevToolsDockSide requested_side = SideFromString(side);
  bool dock_requested = requested_side != DEVTOOLS_DOCK_SIDE_UNDOCKED;
  bool is_docked = IsDocked();

  if (dock_requested &&
      (!GetInspectedWebContents() || !GetInspectedBrowserWindow() ||
       IsInspectedBrowserPopup())) {
    return;
  }

  if ((dock_side_ != DEVTOOLS_DOCK_SIDE_MINIMIZED) &&
      (requested_side == DEVTOOLS_DOCK_SIDE_MINIMIZED))
    dock_side_before_minimized_ = dock_side_;

  dock_side_ = requested_side;
  if (dock_requested && !is_docked) {
    TabStripModel* tab_strip_model = browser_->tab_strip_model();
    tab_strip_model->DetachWebContentsAt(
        tab_strip_model->GetIndexOfWebContents(web_contents_));
    browser_ = NULL;
  } else if (!dock_requested && is_docked) {
    BrowserWindow* inspected_window = GetInspectedBrowserWindow();
    if (inspected_window)
      inspected_window->UpdateDevTools();
  }

  if (dock_side_ != DEVTOOLS_DOCK_SIDE_MINIMIZED) {
    std::string pref_value = kPrefBottom;
    switch (dock_side_) {
      case DEVTOOLS_DOCK_SIDE_UNDOCKED:
          pref_value = kPrefUndocked;
          break;
      case DEVTOOLS_DOCK_SIDE_RIGHT:
          pref_value = kPrefRight;
          break;
      case DEVTOOLS_DOCK_SIDE_BOTTOM:
          pref_value = kPrefBottom;
          break;
      case DEVTOOLS_DOCK_SIDE_MINIMIZED:
          break;
    }
    profile_->GetPrefs()->SetString(prefs::kDevToolsDockSide, pref_value);
  }

  Show(DevToolsToggleAction::Show());
}
