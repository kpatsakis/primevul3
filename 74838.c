void RenderView::OnSetZoomLevelForLoadingURL(const GURL& url,
                                             int zoom_level) {
  host_zoom_levels_[url] = zoom_level;
}
