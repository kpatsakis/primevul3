void ChromotingInstance::SendClipboardItem(const std::string& mime_type,
                                           const std::string& item) {
  if (!IsConnected()) {
    return;
  }
  protocol::ClipboardEvent event;
  event.set_mime_type(mime_type);
  event.set_data(item);
  host_connection_->clipboard_stub()->InjectClipboardEvent(event);
}
