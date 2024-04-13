void RenderView::SyncNavigationState() {
  if (!webview())
    return;

  const WebHistoryItem& item = webview()->mainFrame()->currentHistoryItem();
  if (item.isNull())
    return;

  Send(new ViewHostMsg_UpdateState(
      routing_id_, page_id_, webkit_glue::HistoryItemToString(item)));
}
