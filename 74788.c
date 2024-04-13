void RenderView::OnDownloadFavIcon(int id,
                                   const GURL& image_url,
                                   int image_size) {
  bool data_image_failed = false;
  if (image_url.SchemeIs("data")) {
    SkBitmap data_image = ImageFromDataUrl(image_url);
    data_image_failed = data_image.empty();
    if (!data_image_failed) {
      Send(new ViewHostMsg_DidDownloadFavIcon(routing_id_, id, image_url, false,
                                              data_image));
    }
  }

  if (data_image_failed ||
      !DownloadImage(id, image_url, image_size)) {
    Send(new ViewHostMsg_DidDownloadFavIcon(routing_id_, id, image_url, true,
                                            SkBitmap()));
  }
}
