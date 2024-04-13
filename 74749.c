bool RenderView::DownloadImage(int id, const GURL& image_url, int image_size) {
  if (!webview())
    return false;
  image_fetchers_.insert(new ImageResourceFetcher(
      image_url, webview()->mainFrame(), id, image_size,
      NewCallback(this, &RenderView::DidDownloadImage)));
  return true;
}
