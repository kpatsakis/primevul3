void DevToolsWindow::SetHeight(int height) {
  height_ = height;
  profile_->GetPrefs()->SetInteger(prefs::kDevToolsHSplitLocation, height);
}
