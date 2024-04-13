bool RenderView::AllowContentType(ContentSettingsType settings_type) {
  return current_content_settings_.settings[settings_type] !=
    CONTENT_SETTING_BLOCK;
}
