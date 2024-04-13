void RenderFrameImpl::PluginCrashed(const base::FilePath& plugin_path,
                                   base::ProcessId plugin_pid) {
  Send(new FrameHostMsg_PluginCrashed(routing_id_, plugin_path, plugin_pid));
}
