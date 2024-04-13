void DevToolsWindow::SetWidth(int width) {
  width_ = width;
  profile_->GetPrefs()->SetInteger(prefs::kDevToolsVSplitLocation, width);
}
