void RenderView::UpdateSessionHistory(WebFrame* frame) {
  if (page_id_ == -1)
    return;

  const WebHistoryItem& item =
      webview()->mainFrame()->previousHistoryItem();
  if (item.isNull())
    return;

  Send(new ViewHostMsg_UpdateState(
      routing_id_, page_id_, webkit_glue::HistoryItemToString(item)));
}
