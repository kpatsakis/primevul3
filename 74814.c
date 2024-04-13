void RenderView::OnNotifyRendererViewType(ViewType::Type type) {
  if (view_type_ != ViewType::INVALID) {
    if (type == ViewType::EXTENSION_MOLE ||
        type == ViewType::EXTENSION_TOOLSTRIP) {
      ExtensionProcessBindings::SetViewType(webview(), type);
    }
  }
  view_type_ = type;
}
