void RenderFrameImpl::didFinishDocumentLoad(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  WebDataSource* ds = frame->dataSource();
  DocumentState* document_state = DocumentState::FromDataSource(ds);
  document_state->set_finish_document_load_time(Time::Now());

  Send(new FrameHostMsg_DidFinishDocumentLoad(routing_id_));

  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    DidFinishDocumentLoad(frame));
  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, DidFinishDocumentLoad());

  render_view_->UpdateEncoding(frame, frame->view()->pageEncoding().utf8());
}
