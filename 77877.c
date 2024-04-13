void RenderFrameImpl::SetWebFrame(blink::WebLocalFrame* web_frame) {
  DCHECK(!frame_);

  std::pair<FrameMap::iterator, bool> result = g_frame_map.Get().insert(
      std::make_pair(web_frame, this));
  CHECK(result.second) << "Inserting a duplicate item.";

  frame_ = web_frame;
}
