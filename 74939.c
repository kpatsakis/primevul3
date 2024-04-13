int RenderView::historyForwardListCount() {
  return history_list_length_ - historyBackListCount() - 1;
}
