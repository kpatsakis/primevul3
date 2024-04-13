PrintWebViewHelper::PrintPreviewContext::GetPrintCanvasSize() const {
  DCHECK(IsRendering());
  return prep_frame_view_->GetPrintCanvasSize();
}
