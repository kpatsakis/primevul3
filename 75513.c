const SkBitmap& ExtensionService::GetOmniboxPopupIcon(
    const std::string& extension_id) {
  return omnibox_popup_icon_manager_.GetIcon(extension_id);
}
