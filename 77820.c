void RenderFrameImpl::DidPlay(blink::WebMediaPlayer* player) {
  Send(new FrameHostMsg_MediaPlayingNotification(
      routing_id_, reinterpret_cast<int64>(player), player->hasVideo(),
      player->hasAudio()));
}
