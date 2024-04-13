void RenderFrameImpl::DidPause(blink::WebMediaPlayer* player) {
  Send(new FrameHostMsg_MediaPausedNotification(
      routing_id_, reinterpret_cast<int64>(player)));
}
