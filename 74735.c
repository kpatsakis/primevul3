WebPlugin* RenderView::CreatePluginNoCheck(WebFrame* frame,
                                           const WebPluginParams& params) {
  WebPluginInfo info;
  bool found;
  std::string mime_type;
  Send(new ViewHostMsg_GetPluginInfo(
      params.url, frame->top()->url(), params.mimeType.utf8(), &found,
      &info, &mime_type));
  if (!found || !info.enabled)
    return NULL;
  scoped_refptr<pepper::PluginModule> pepper_module =
      PepperPluginRegistry::GetInstance()->GetModule(info.path);
  if (pepper_module)
    return CreatePepperPlugin(frame, params, info.path, pepper_module.get());
  else
    return CreateNPAPIPlugin(frame, params, info.path, mime_type);
}
