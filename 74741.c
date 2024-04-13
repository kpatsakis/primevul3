void RenderView::DidBlockContentType(ContentSettingsType settings_type) {
  if (!content_blocked_[settings_type]) {
    content_blocked_[settings_type] = true;
    Send(new ViewHostMsg_ContentBlocked(routing_id_, settings_type));
  }
}
