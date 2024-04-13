void MediaStreamImpl::requestUserMedia(
    const WebKit::WebUserMediaRequest& user_media_request,
    const WebKit::WebVector<WebKit::WebMediaStreamSource>&
        media_stream_source_vector) {
  DCHECK(CalledOnValidThread());
  DCHECK(!user_media_request.isNull());

  int request_id = next_request_id_++;

  bool audio = user_media_request.audio();
  media_stream::StreamOptions::VideoOption video_option =
      media_stream::StreamOptions::kNoCamera;
  if (user_media_request.video()) {
    video_option = media_stream::StreamOptions::kFacingUser;
    if (user_media_request.cameraPreferenceUser() &&
        user_media_request.cameraPreferenceEnvironment()) {
      video_option = media_stream::StreamOptions::kFacingBoth;
    } else if (user_media_request.cameraPreferenceEnvironment()) {
      video_option = media_stream::StreamOptions::kFacingEnvironment;
    }
  }

  std::string security_origin = UTF16ToUTF8(
      user_media_request.securityOrigin().toString());

  DVLOG(1) << "MediaStreamImpl::generateStream(" << request_id << ", [ "
           << (audio ? "audio " : "")
           << ((user_media_request.cameraPreferenceUser()) ?
               "video_facing_user " : "")
           << ((user_media_request.cameraPreferenceEnvironment()) ?
               "video_facing_environment " : "") << "], "
           << security_origin << ")";

  user_media_requests_.insert(
      std::pair<int, WebKit::WebUserMediaRequest>(
          request_id, user_media_request));

  media_stream_dispatcher_->GenerateStream(
      request_id,
      this,
      media_stream::StreamOptions(audio, video_option),
      security_origin);
}
