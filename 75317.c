void PrintWebViewHelper::PrintPreviewContext::OnPrintPreview() {
  DCHECK_EQ(INITIALIZED, state_);
  ClearContext();
}
