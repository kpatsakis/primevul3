void RenderFrameImpl::didChangeName(blink::WebLocalFrame* frame,
                                    const blink::WebString& name) {
  DCHECK(!frame_ || frame_ == frame);
  if (!render_view_->renderer_preferences_.report_frame_name_changes)
    return;

  render_view_->Send(
      new ViewHostMsg_UpdateFrameName(render_view_->GetRoutingID(),
                                      routing_id_,
                                      !frame->parent(),
                                      base::UTF16ToUTF8(name)));
}
