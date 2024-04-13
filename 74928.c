void RenderView::didStopLoading() {
  if (!is_loading_) {
    DLOG(WARNING) << "DidStopLoading called while not loading";
    return;
  }

  is_loading_ = false;

  GURL favicon_url(webview()->mainFrame()->favIconURL());
  if (!favicon_url.is_empty())
    Send(new ViewHostMsg_UpdateFavIconURL(routing_id_, page_id_, favicon_url));

  AddGURLSearchProvider(webview()->mainFrame()->openSearchDescriptionURL(),
                        true);  // autodetected

  Send(new ViewHostMsg_DidStopLoading(routing_id_));

  MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      method_factory_.NewRunnableMethod(&RenderView::CapturePageInfo, page_id_,
                                        false),
      kDelayForCaptureMs);
}
