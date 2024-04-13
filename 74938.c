int RenderView::historyBackListCount() {
  return history_list_offset_ < 0 ? 0 : history_list_offset_;
}
