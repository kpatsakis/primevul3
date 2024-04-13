bool WebContentsImpl::AddMessageToConsole(int32 level,
                                          const string16& message,
                                          int32 line_no,
                                          const string16& source_id) {
  if (!delegate_)
    return false;
  return delegate_->AddMessageToConsole(this, level, message, line_no,
                                        source_id);
}
