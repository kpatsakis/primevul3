int WebContentsImpl::GetZoomPercent(bool* enable_increment,
                                    bool* enable_decrement) const {
  *enable_decrement = *enable_increment = false;
  int percent = static_cast<int>(
      ZoomLevelToZoomFactor(GetZoomLevel()) * 100 + 0.5);
  *enable_decrement = percent > minimum_zoom_percent_;
  *enable_increment = percent < maximum_zoom_percent_;
  return percent;
}
