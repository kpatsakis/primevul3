int PrintWebViewHelper::PrintPreviewContext::total_page_count() const {
  DCHECK(IsRendering());
  return total_page_count_;
}
