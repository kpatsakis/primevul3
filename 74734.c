webkit_glue::WebPluginDelegate* RenderView::CreatePluginDelegate(
    const FilePath& file_path,
    const std::string& mime_type) {
  if (!PluginChannelHost::IsListening())
    return NULL;

  bool use_pepper_host = false;
  bool in_process_plugin = RenderProcess::current()->UseInProcessPlugins();
  const char kPepperPrefix[] = "pepper-";
  if (StartsWithASCII(mime_type, kPepperPrefix, true)) {
    if (CommandLine::ForCurrentProcess()->
            HasSwitch(switches::kInternalPepper)) {
      in_process_plugin = true;
      use_pepper_host = true;
    } else {
      return NULL;
    }
  } else {
    FilePath internal_pdf_path;
    PathService::Get(chrome::FILE_PDF_PLUGIN, &internal_pdf_path);
    if (file_path == internal_pdf_path) {
      in_process_plugin = true;
      use_pepper_host = true;
    }
  }

  if (mime_type == "application/x-nacl-srpc") {

    GURL main_frame_url(webview()->mainFrame()->url());
    const std::string &extension_id =
        RenderThread::current()->GetExtensionIdByURL(main_frame_url);
    bool in_ext = extension_id != "";
    bool explicit_enable =
        CommandLine::ForCurrentProcess()->HasSwitch(switches::kInternalNaCl);

    if (in_ext) {
      if (ExtensionProcessBindings::HasPermission(extension_id,
              Extension::kNativeClientPermission)) {
        in_process_plugin = true;
        use_pepper_host = true;
      } else {
        return NULL;
      }
    } else if (explicit_enable) {
      in_process_plugin = true;
      use_pepper_host = true;
    }
  }

  if (in_process_plugin) {
    if (use_pepper_host) {
      WebPluginDelegatePepper* pepper_plugin =
           WebPluginDelegatePepper::Create(file_path, mime_type, AsWeakPtr());
      if (!pepper_plugin)
        return NULL;

      current_oldstyle_pepper_plugins_.insert(pepper_plugin);
      return pepper_plugin;
    } else {
#if defined(OS_WIN)  // In-proc plugins aren't supported on Linux or Mac.
      return WebPluginDelegateImpl::Create(
          file_path, mime_type, gfx::NativeViewFromId(host_window_));
#else
      NOTIMPLEMENTED();
      return NULL;
#endif
    }
  }

  return new WebPluginDelegateProxy(mime_type, AsWeakPtr());
}
