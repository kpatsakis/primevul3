RenderView* RenderView::FromWebView(WebView* webview) {
  ViewMap* views = Singleton<ViewMap>::get();
  ViewMap::iterator it = views->find(webview);
  return it == views->end() ? NULL : it->second;
}
