void PrintWebViewHelper::PrintPreviewContext::Finished() {
  DCHECK_EQ(DONE, state_);
  state_ = INITIALIZED;
  ClearContext();
}
