void RenderThreadImpl::OnSetZoomLevelForCurrentURL(const std::string& scheme,
                                                   const std::string& host,
                                                   double zoom_level) {
  RenderViewZoomer zoomer(scheme, host, zoom_level);
  RenderView::ForEach(&zoomer);
}
