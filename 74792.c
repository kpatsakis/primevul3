void RenderView::OnDragTargetDragOver(const gfx::Point& client_point,
                                      const gfx::Point& screen_point,
                                      WebDragOperationsMask ops) {
  WebDragOperation operation = webview()->dragTargetDragOver(
      client_point,
      screen_point,
      ops);

  Send(new ViewHostMsg_UpdateDragCursor(routing_id_, operation));
}
