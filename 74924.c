void RenderView::didReceiveTitle(WebFrame* frame, const WebString& title) {
  UpdateTitle(frame, title);

  UpdateEncoding(frame, frame->view()->pageEncoding().utf8());
}
