void RenderView::reportFindInPageMatchCount(int request_id, int count,
                                            bool final_update) {
  if (queued_find_reply_message_.get()) {
    IPC::Message* msg = new ViewHostMsg_Find_Reply(
        routing_id_,
        request_id,
        count,
        gfx::Rect(),
        -1,  // Don't update active match ordinal.
        final_update);
    queued_find_reply_message_.reset(msg);
  } else {
    Send(new ViewHostMsg_Find_Reply(
        routing_id_,
        request_id,
        count,
        gfx::Rect(),
        -1,  // Don't update active match ordinal.
        final_update));
  }
}
