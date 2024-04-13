RenderFrame* RenderFrame::FromWebFrame(blink::WebFrame* web_frame) {
  return RenderFrameImpl::FromWebFrame(web_frame);
}
