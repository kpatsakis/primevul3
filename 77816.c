WebMediaPlayer* RenderFrameImpl::CreateWebMediaPlayerForMediaStream(
    const blink::WebURL& url,
    WebMediaPlayerClient* client) {
#if defined(ENABLE_WEBRTC)
  if (!InitializeMediaStreamClient()) {
    LOG(ERROR) << "Failed to initialize MediaStreamClient";
    return NULL;
  }
  if (media_stream_client_->IsMediaStream(url)) {
#if defined(OS_ANDROID) && defined(ARCH_CPU_ARMEL)
    bool found_neon =
        (android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
    UMA_HISTOGRAM_BOOLEAN("Platform.WebRtcNEONFound", found_neon);
#endif  // defined(OS_ANDROID) && defined(ARCH_CPU_ARMEL)
    return new WebMediaPlayerMS(frame_, client, weak_factory_.GetWeakPtr(),
                                media_stream_client_, new RenderMediaLog());
  }
#endif  // defined(ENABLE_WEBRTC)
  return NULL;
}
