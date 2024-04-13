blink::WebMediaStreamCenter* RenderThreadImpl::CreateMediaStreamCenter(
    blink::WebMediaStreamCenterClient* client) {
#if defined(OS_ANDROID)
  if (CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableWebRTC))
    return NULL;
#endif

#if defined(ENABLE_WEBRTC)
  if (!media_stream_center_) {
    media_stream_center_ = GetContentClient()->renderer()
        ->OverrideCreateWebMediaStreamCenter(client);
    if (!media_stream_center_) {
      scoped_ptr<MediaStreamCenter> media_stream_center(
          new MediaStreamCenter(client, GetPeerConnectionDependencyFactory()));
      AddObserver(media_stream_center.get());
      media_stream_center_ = media_stream_center.release();
    }
  }
#endif
  return media_stream_center_;
}
