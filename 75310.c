bool PrintWebViewHelper::PrintPreviewContext::IsRendering() const {
  return state_ == RENDERING || state_ == DONE;
}
