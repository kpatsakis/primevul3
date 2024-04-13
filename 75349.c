bool PluginInfoBarDelegate::Cancel() {
  tab_contents_->render_view_host()->Send(new ViewMsg_LoadBlockedPlugins(
      tab_contents_->render_view_host()->routing_id()));
  return true;
}
