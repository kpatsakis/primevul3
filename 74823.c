void RenderView::OnResize(const gfx::Size& new_size,
                          const gfx::Rect& resizer_rect) {
  if (webview()) {
    webview()->hidePopups();

    if (send_preferred_size_changes_) {
      bool allow_scrollbars = (
          disable_scrollbars_size_limit_.width() <= new_size.width() ||
          disable_scrollbars_size_limit_.height() <= new_size.height());
      webview()->mainFrame()->setCanHaveScrollbars(allow_scrollbars);
    }
  }

  RenderWidget::OnResize(new_size, resizer_rect);
}
