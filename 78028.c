int WebContentsImpl::DownloadImage(const GURL& url,
                                   bool is_favicon,
                                   uint32_t max_bitmap_size,
                                   const ImageDownloadCallback& callback) {
  RenderViewHost* host = GetRenderViewHost();
  int id = StartDownload(host, url, is_favicon, max_bitmap_size);
  image_download_map_[id] = callback;
  return id;
}
