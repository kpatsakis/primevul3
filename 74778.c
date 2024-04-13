void RenderView::OnCaptureThumbnail() {
  WebFrame* main_frame = webview()->mainFrame();
  if (!main_frame)
    return;

  GURL url(main_frame->url());
  if (url.is_empty())
    return;

  if (size_.IsEmpty())
    return;  // Don't create an empty thumbnail!

  ThumbnailScore score;
  SkBitmap thumbnail;
  if (!CaptureThumbnail(webview(), kThumbnailWidth, kThumbnailHeight,
                        &thumbnail, &score))
    return;

  Send(new ViewHostMsg_Thumbnail(routing_id_, url, score, thumbnail));
}
