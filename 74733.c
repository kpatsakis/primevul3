WebPlugin* RenderView::CreatePepperPlugin(WebFrame* frame,
                                          const WebPluginParams& params,
                                          const FilePath& path,
                                          pepper::PluginModule* pepper_module) {
  WebPlugin* plugin = new pepper::WebPluginImpl(pepper_module, params,
                                                pepper_delegate_.AsWeakPtr());
  if (plugin && !frame->parent() && frame->document().isPluginDocument()) {
    FilePath pdf_path;
    PathService::Get(chrome::FILE_PDF_PLUGIN, &pdf_path);
    if (path == pdf_path)
      Send(new ViewHostMsg_SetDisplayingPDFContent(routing_id_));
  }
  return plugin;
}
