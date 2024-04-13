void RenderView::didHandleOnloadEvents(WebFrame* frame) {
  if (webview()->mainFrame() == frame)
    Send(new ViewHostMsg_DocumentOnLoadCompletedInMainFrame(routing_id_));
}
