void PrintWebViewHelper::PrintPreviewContext::ClearContext() {
  prep_frame_view_.reset();
  metafile_.reset();
  pages_to_render_.clear();
  error_ = PREVIEW_ERROR_NONE;
}
