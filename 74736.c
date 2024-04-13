WebPlugin* RenderView::CreatePluginPlaceholder(WebFrame* frame,
                                               const WebPluginParams& params,
                                               PluginGroup* group) {
  BlockedPlugin* blocked_plugin = new BlockedPlugin(this, frame, params, group);
  WebViewPlugin* plugin = blocked_plugin->plugin();
  WebView* web_view = plugin->web_view();
  webkit_preferences_.Apply(web_view);
  return plugin;
}
