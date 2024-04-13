void RenderView::OnMissingPluginStatus(
    WebPluginDelegateProxy* delegate,
    int status) {
#if defined(OS_WIN)
  if (!first_default_plugin_) {
    if (status == default_plugin::MISSING_PLUGIN_AVAILABLE) {
      first_default_plugin_ = delegate->AsWeakPtr();
      Send(new ViewHostMsg_MissingPluginStatus(routing_id_, status));
    }
  } else {
    if (status == default_plugin::MISSING_PLUGIN_USER_STARTED_DOWNLOAD) {
      Send(new ViewHostMsg_MissingPluginStatus(routing_id_, status));
    }
  }
#else
  NOTIMPLEMENTED();
#endif
}
