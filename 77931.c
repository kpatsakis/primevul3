blink::WebHistoryItem RenderFrameImpl::historyItemForNewChildFrame(
    blink::WebFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  return render_view_->history_controller()->GetItemForNewChildFrame(this);
}
