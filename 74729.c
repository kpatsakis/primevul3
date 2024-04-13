void RenderView::ClearBlockedContentSettings() {
  for (size_t i = 0; i < arraysize(content_blocked_); ++i)
    content_blocked_[i] = false;
}
