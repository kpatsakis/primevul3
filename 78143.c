void WebContentsImpl::OnPepperPluginHung(int plugin_child_id,
                                         const base::FilePath& path,
                                         bool is_hung) {
  UMA_HISTOGRAM_COUNTS("Pepper.PluginHung", 1);

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    PluginHungStatusChanged(plugin_child_id, path, is_hung));
}
