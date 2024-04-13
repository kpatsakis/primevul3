void RenderView::didCreateDocumentElement(WebFrame* frame) {
  if (RenderThread::current()) {  // Will be NULL during unit tests.
    RenderThread::current()->user_script_slave()->InjectScripts(
        frame, UserScript::DOCUMENT_START);
  }
  if (view_type_ == ViewType::EXTENSION_TOOLSTRIP ||
      view_type_ == ViewType::EXTENSION_MOLE) {
    InjectToolstripCSS();
    ExtensionProcessBindings::SetViewType(webview(), view_type_);
  }

  GURL url = frame->url();
  if (url.is_valid() && url.spec() != chrome::kAboutBlankURL) {
    if (frame == webview()->mainFrame())
      Send(new ViewHostMsg_DocumentAvailableInMainFrame(routing_id_));
  }
}
