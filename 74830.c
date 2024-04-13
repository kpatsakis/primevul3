void RenderView::OnSetContentSettingsForLoadingURL(
    const GURL& url,
    const ContentSettings& content_settings) {
  host_content_settings_[url] = content_settings;
}
