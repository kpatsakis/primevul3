void RenderViewImpl::EnsureMediaStreamImpl() {
  if (!RenderThreadImpl::current())  // Will be NULL during unit tests.
    return;

#if defined(OS_ANDROID)
  if (!CommandLine::ForCurrentProcess()->HasSwitch(switches::kEnableWebRTC))
    return;
#endif

#if defined(ENABLE_WEBRTC)
  if (!media_stream_dispatcher_)
    media_stream_dispatcher_ = new MediaStreamDispatcher(this);

  if (!media_stream_impl_) {
    media_stream_impl_ = new MediaStreamImpl(
        this,
        media_stream_dispatcher_,
        RenderThreadImpl::current()->video_capture_impl_manager(),
        RenderThreadImpl::current()->GetMediaStreamDependencyFactory());
  }
#endif
}
