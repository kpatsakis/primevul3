void WebContentsImpl::OnCrashedPlugin(const base::FilePath& plugin_path,
                                      base::ProcessId plugin_pid) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    PluginCrashed(plugin_path, plugin_pid));
}
