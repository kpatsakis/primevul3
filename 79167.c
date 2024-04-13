int DevToolsWindow::GetHeight(int container_height) {
  if (height_ == -1) {
    height_ = profile_->GetPrefs()->
        GetInteger(prefs::kDevToolsHSplitLocation);
  }

  if (height_ == -1)
    height_ = container_height / 3;

  height_ = std::max(GetMinimumHeight(), height_);

  height_ = std::min(container_height - kMinContentsSize, height_);
  return height_;
}
