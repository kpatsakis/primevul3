void RenderView::OnReservePageIDRange(int size_of_range) {
  next_page_id_ += size_of_range + 1;
}
