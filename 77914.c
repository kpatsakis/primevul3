void RenderFrameImpl::didFirstVisuallyNonEmptyLayout(
    blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  if (frame->parent())
    return;

  InternalDocumentStateData* data =
      InternalDocumentStateData::FromDataSource(frame->dataSource());
  data->set_did_first_visually_non_empty_layout(true);

#if defined(OS_ANDROID)
  GetRenderWidget()->DidChangeBodyBackgroundColor(
      render_view_->webwidget_->backgroundColor());
#endif
}
