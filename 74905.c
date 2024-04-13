void RenderView::didCompleteClientRedirect(
    WebFrame* frame, const WebURL& from) {
  if (!frame->parent())
    completed_client_redirect_src_ = from;
}
