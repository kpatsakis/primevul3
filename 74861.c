void RenderView::ReportNoFindInPageResults(int request_id) {
  Send(new ViewHostMsg_Find_Reply(routing_id_,
                                  request_id,
                                  0,
                                  gfx::Rect(),
                                  0,
                                  true));
}
