RenderFrameImpl* RenderFrameImpl::FromWebFrame(blink::WebFrame* web_frame) {
  FrameMap::iterator iter = g_frame_map.Get().find(web_frame);
  if (iter != g_frame_map.Get().end())
    return iter->second;
  return NULL;
}
