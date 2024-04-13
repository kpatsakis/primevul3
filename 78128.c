void WebContentsImpl::OnDidDownloadImage(
    int id,
    int http_status_code,
    const GURL& image_url,
    const std::vector<SkBitmap>& bitmaps,
    const std::vector<gfx::Size>& original_bitmap_sizes) {
  ImageDownloadMap::iterator iter = image_download_map_.find(id);
  if (iter == image_download_map_.end()) {
    return;
  }
  if (!iter->second.is_null()) {
    iter->second.Run(
        id, http_status_code, image_url, bitmaps, original_bitmap_sizes);
  }
  image_download_map_.erase(id);
}
