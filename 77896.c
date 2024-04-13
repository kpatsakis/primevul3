void RenderFrameImpl::didAccessInitialDocument(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  if (!frame->parent() && render_view_->page_id_ == -1)
    Send(new FrameHostMsg_DidAccessInitialDocument(routing_id_));
}
