void RenderView::PluginCrashed(const FilePath& plugin_path) {
  Send(new ViewHostMsg_CrashedPlugin(routing_id_, plugin_path));
}
