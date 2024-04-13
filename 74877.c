bool RenderView::allowDatabase(
    WebFrame* frame, const WebString& name, const WebString& display_name,
    unsigned long estimated_size) {
  WebSecurityOrigin origin = frame->securityOrigin();
  if (origin.isEmpty())
    return false;  // Uninitialized document?

  bool result;
  if (!Send(new ViewHostMsg_AllowDatabase(routing_id_,
      origin.toString().utf8(), name, display_name, estimated_size, &result)))
    return false;
  Send(new ViewHostMsg_WebDatabaseAccessed(routing_id_,
                                           GURL(origin.toString().utf8()),
                                           name,
                                           display_name,
                                           estimated_size,
                                           !result));
  return result;
}
