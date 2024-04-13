void RenderView::DidDownloadImage(ImageResourceFetcher* fetcher,
                                  const SkBitmap& image) {
  Send(new ViewHostMsg_DidDownloadFavIcon(routing_id_,
                                          fetcher->id(),
                                          fetcher->image_url(),
                                          image.isNull(),
                                          image));
  DCHECK(image_fetchers_.find(fetcher) != image_fetchers_.end());
  image_fetchers_.erase(fetcher);
  MessageLoop::current()->DeleteSoon(FROM_HERE, fetcher);
}
