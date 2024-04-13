void RenderView::reportFindInPageSelection(int request_id,
                                           int active_match_ordinal,
                                           const WebRect& selection_rect) {
  Send(new ViewHostMsg_Find_Reply(routing_id_,
                                  request_id,
                                  -1,
                                  selection_rect,
                                  active_match_ordinal,
                                  false));
}
