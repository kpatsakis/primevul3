WebKit::WebPlugin* RenderViewImpl::CreatePlugin(
    WebKit::WebFrame* frame,
    const webkit::WebPluginInfo& info,
    const WebKit::WebPluginParams& params) {
  WebKit::WebPlugin* pepper_webplugin =
      pepper_helper_->CreatePepperWebPlugin(info, params);

  if (pepper_webplugin)
    return pepper_webplugin;

  if (!webkit::npapi::NPAPIPluginsSupported())
    return NULL;

  return new webkit::npapi::WebPluginImpl(
      frame, params, info.path, AsWeakPtr());
}
