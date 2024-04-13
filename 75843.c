bool NavigationController::CanViewSource() const {
  bool is_supported_mime_type = net::IsSupportedNonImageMimeType(
      tab_contents_->contents_mime_type().c_str());
  NavigationEntry* active_entry = GetActiveEntry();
  return active_entry && !active_entry->IsViewSourceMode() &&
    is_supported_mime_type;
}
