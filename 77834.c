bool RenderFrameImpl::InitializeMediaStreamClient() {
  if (media_stream_client_)
    return true;

  if (!RenderThreadImpl::current())  // Will be NULL during unit tests.
    return false;

#if defined(OS_ANDROID)
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kDisableWebRTC))
    return false;
#endif

#if defined(ENABLE_WEBRTC)
  if (!render_view_->media_stream_dispatcher_) {
    render_view_->media_stream_dispatcher_ =
        new MediaStreamDispatcher(render_view_.get());
  }

  MediaStreamImpl* media_stream_impl = new MediaStreamImpl(
      render_view_.get(),
      render_view_->media_stream_dispatcher_,
      RenderThreadImpl::current()->GetMediaStreamDependencyFactory());
  media_stream_client_ = media_stream_impl;
  web_user_media_client_ = media_stream_impl;
  return true;
#else
  return false;
#endif
}
