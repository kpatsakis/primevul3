int PrintWebViewHelper::PrintPreviewContext::GetNextPageNumber() {
  DCHECK_EQ(RENDERING, state_);
  if (IsFinalPageRendered())
    return -1;
  return pages_to_render_[current_page_index_++];
}
