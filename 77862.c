void RenderFrameImpl::OnSwapOut() {
  if (!is_swapped_out_ || !render_view_->is_swapped_out_) {

    render_view_->SyncNavigationState();

    if (!frame_->parent())
      frame_->dispatchUnloadEvent();

    if (!frame_->parent())
      render_view_->SetSwappedOut(true);
    is_swapped_out_ = true;

    if (!frame_->parent())
      render_view_->OnStop();
    else
      frame_->stopLoading();

    if (frame_->parent())
      frame_->setIsRemote(true);

    render_view_->NavigateToSwappedOutURL(frame_);

    if (frame_->parent())
      render_view_->RegisterSwappedOutChildFrame(this);

    if (!frame_->parent()) {
      render_view_->webview()->setVisibilityState(
          blink::WebPageVisibilityStateHidden, false);
    }
  }

  if (!frame_->parent())
    render_view_->suppress_dialogs_until_swap_out_ = false;

  Send(new FrameHostMsg_SwapOut_ACK(routing_id_));
}
