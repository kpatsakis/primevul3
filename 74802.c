void RenderView::OnFindReplyAck() {
  if (queued_find_reply_message_.get()) {
    Send(queued_find_reply_message_.release());
  }
}
