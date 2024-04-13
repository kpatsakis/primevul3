WebPlugin* RenderView::CreateNPAPIPlugin(WebFrame* frame,
                                         const WebPluginParams& params,
                                         const FilePath& path,
                                         const std::string& mime_type) {
  std::string actual_mime_type(mime_type);
  if (actual_mime_type.empty())
    actual_mime_type = params.mimeType.utf8();

  return new webkit_glue::WebPluginImpl(frame, params, path,
                                        actual_mime_type, AsWeakPtr());
}
