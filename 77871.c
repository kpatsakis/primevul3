void RenderFrameImpl::PlayerGone(blink::WebMediaPlayer* player) {
  DidPause(player);
}
