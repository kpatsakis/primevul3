void RenderView::Close() {
  WebView* doomed = webview();
  RenderWidget::Close();
  Singleton<ViewMap>::get()->erase(doomed);
}
