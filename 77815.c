blink::WebPlugin* RenderFrameImpl::CreatePlugin(
    blink::WebFrame* frame,
    const WebPluginInfo& info,
    const blink::WebPluginParams& params) {
  DCHECK_EQ(frame_, frame);
#if defined(ENABLE_PLUGINS)
  bool pepper_plugin_was_registered = false;
  scoped_refptr<PluginModule> pepper_module(PluginModule::Create(
      this, info, &pepper_plugin_was_registered));
  if (pepper_plugin_was_registered) {
    if (pepper_module.get()) {
      return new PepperWebPluginImpl(pepper_module.get(), params, this);
    }
  }
#if defined(OS_CHROMEOS)
  LOG(WARNING) << "Pepper module/plugin creation failed.";
  return NULL;
#else
  return new WebPluginImpl(frame, params, info.path, render_view_, this);
#endif
#else
  return NULL;
#endif
}
