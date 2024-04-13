void RenderView::OnUpdateWebPreferences(const WebPreferences& prefs) {
  webkit_preferences_ = prefs;
  webkit_preferences_.Apply(webview());
}
