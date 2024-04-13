scoped_refptr<media::VideoDecoder> MediaStreamImpl::GetVideoDecoder(
    const GURL& url,
    media::MessageLoopFactory* message_loop_factory) {
  DCHECK(CalledOnValidThread());
  WebKit::WebMediaStreamDescriptor descriptor(
      WebKit::WebMediaStreamRegistry::lookupMediaStreamDescriptor(url));
  if (descriptor.isNull())
    return NULL;  // This is not a valid stream.


  WebKit::WebVector<WebKit::WebMediaStreamSource> source_vector;
  descriptor.sources(source_vector);
  std::string msm_label;
  for (size_t i = 0; i < source_vector.size(); ++i) {
    if (source_vector[i].type() == WebKit::WebMediaStreamSource::TypeVideo) {
      msm_label = ExtractManagerStreamLabel(UTF16ToUTF8(source_vector[i].id()));
      break;
    }
  }
  if (msm_label.empty())
    return NULL;

  scoped_refptr<media::VideoDecoder> decoder;
  if (media_stream_dispatcher_->IsStream(msm_label)) {
    int video_session_id =
        media_stream_dispatcher_->video_session_id(msm_label, 0);
    media::VideoCapture::VideoCaptureCapability capability;
    capability.width = kVideoCaptureWidth;
    capability.height = kVideoCaptureHeight;
    capability.max_fps = kVideoCaptureFramePerSecond;
    capability.expected_capture_delay = 0;
    capability.raw_type = media::VideoFrame::I420;
    capability.interlaced = false;
    decoder = new CaptureVideoDecoder(
        message_loop_factory->GetMessageLoopProxy("CaptureVideoDecoderThread"),
        video_session_id,
        vc_manager_.get(),
        capability);
  } else {
    if (!video_renderer_.get())
      video_renderer_ = new talk_base::RefCountedObject<VideoRendererWrapper>();
    if (video_renderer_->renderer()) {
      if (peer_connection_handler_) {
        peer_connection_handler_->SetVideoRenderer(
            UTF16ToUTF8(descriptor.label()),
            NULL);
      }
      video_renderer_->SetVideoDecoder(NULL);
    }
    RTCVideoDecoder* rtc_video_decoder = new RTCVideoDecoder(
        message_loop_factory->GetMessageLoop("RtcVideoDecoderThread"),
        url.spec());
    decoder = rtc_video_decoder;
    video_renderer_->SetVideoDecoder(rtc_video_decoder);
    if (peer_connection_handler_) {
      peer_connection_handler_->SetVideoRenderer(
          UTF16ToUTF8(descriptor.label()),
          video_renderer_);
    }
  }
  return decoder;
}
