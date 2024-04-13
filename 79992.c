void RenderViewImpl::GetSelectionBounds(gfx::Rect* start, gfx::Rect* end) {
  if (pepper_helper_->IsPluginFocused()) {
    gfx::Rect caret = pepper_helper_->GetCaretBounds();
    *start = caret;
    *end = caret;
    return;
  }
  RenderWidget::GetSelectionBounds(start, end);
}
