void PrintWebViewHelper::PrintPreviewContext::Failed(bool report_error) {
  DCHECK(state_ == INITIALIZED || state_ == RENDERING);
  state_ = INITIALIZED;
  if (report_error) {
    DCHECK_NE(PREVIEW_ERROR_NONE, error_);
    UMA_HISTOGRAM_ENUMERATION("PrintPreview.RendererError", error_,
                              PREVIEW_ERROR_LAST_ENUM);
  }
  ClearContext();
}
