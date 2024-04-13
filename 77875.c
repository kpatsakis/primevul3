bool RenderFrameImpl::Send(IPC::Message* message) {
  if (is_detaching_ ||
      ((is_swapped_out_ || render_view_->is_swapped_out()) &&
       !SwappedOutMessages::CanSendWhileSwappedOut(message))) {
    delete message;
    return false;
  }

  return RenderThread::Get()->Send(message);
 }
