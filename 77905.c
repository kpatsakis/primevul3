void RenderFrameImpl::didCreateDocumentElement(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);

  GURL url = frame->document().url();
  if (url.is_valid() && url.spec() != kAboutBlankURL) {
    if (frame == render_view_->webview()->mainFrame()) {
      render_view_->Send(new ViewHostMsg_DocumentAvailableInMainFrame(
          render_view_->GetRoutingID()));
    }
  }

  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    DidCreateDocumentElement(frame));
}
