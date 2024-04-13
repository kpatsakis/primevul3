void RenderView::SetNextPageID(int32 next_page_id) {
  DCHECK_EQ(next_page_id_, 1);
  DCHECK(next_page_id >= next_page_id_);
  next_page_id_ = next_page_id;
}
