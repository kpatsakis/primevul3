void MediaStreamImpl::OnStreamGenerated(
    int request_id,
    const std::string& label,
    const media_stream::StreamDeviceInfoArray& audio_array,
    const media_stream::StreamDeviceInfoArray& video_array) {
  DCHECK(CalledOnValidThread());
  EnsurePeerConnectionFactory();

  WebKit::WebVector<WebKit::WebMediaStreamSource> source_vector(
      audio_array.size() + video_array.size());

  std::string track_label;
  for (size_t i = 0; i < audio_array.size(); ++i) {
    track_label = CreateTrackLabel(label, audio_array[i].session_id, false);
    MediaStreamTrackPtr audio_track(
        dependency_factory_->CreateLocalAudioTrack(audio_array[i].name, NULL));
    local_tracks_.insert(
        std::pair<std::string, MediaStreamTrackPtr>(track_label, audio_track));
    source_vector[i].initialize(
          UTF8ToUTF16(track_label),
          WebKit::WebMediaStreamSource::TypeAudio,
          UTF8ToUTF16(audio_array[i].name));
  }

  for (size_t i = 0; i < video_array.size(); ++i) {
    track_label = CreateTrackLabel(label, video_array[i].session_id, true);
    webrtc::VideoCaptureModule* vcm =
        new VideoCaptureModuleImpl(video_array[i].session_id,
                                   vc_manager_.get());
    MediaStreamTrackPtr video_track(dependency_factory_->CreateLocalVideoTrack(
            video_array[i].name,
            webrtc::CreateVideoCapturer(vcm)));
    local_tracks_.insert(
        std::pair<std::string, MediaStreamTrackPtr>(track_label, video_track));
    source_vector[audio_array.size() + i].initialize(
          UTF8ToUTF16(track_label),
          WebKit::WebMediaStreamSource::TypeVideo,
          UTF8ToUTF16(video_array[i].name));
  }


  MediaRequestMap::iterator it = user_media_requests_.find(request_id);
  if (it == user_media_requests_.end()) {
    DVLOG(1) << "Request ID not found";
    return;
  }
  WebKit::WebUserMediaRequest user_media_request = it->second;
  user_media_requests_.erase(it);

  user_media_request.requestSucceeded(source_vector);
}
