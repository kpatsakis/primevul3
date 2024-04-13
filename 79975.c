webkit::npapi::WebPluginDelegate* RenderViewImpl::CreatePluginDelegate(
    const base::FilePath& file_path,
    const std::string& mime_type) {
  if (!PluginChannelHost::IsListening()) {
    LOG(ERROR) << "PluginChannelHost isn't listening";
    return NULL;
  }

  bool in_process_plugin = RenderProcess::current()->UseInProcessPlugins();
  if (in_process_plugin) {
#if defined(OS_WIN) && !defined(USE_AURA)
    return webkit::npapi::WebPluginDelegateImpl::Create(file_path, mime_type);
#else
    NOTIMPLEMENTED();
    return NULL;
#endif
  }

  return new WebPluginDelegateProxy(mime_type, AsWeakPtr());
}
