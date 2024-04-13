void MediaStreamImpl::ClosePeerConnection() {
  DCHECK(CalledOnValidThread());
  video_renderer_ = NULL;
  peer_connection_handler_ = NULL;
  MediaStreamTrackPtrMap::const_iterator it = local_tracks_.begin();
  for (; it != local_tracks_.end(); ++it)
    it->second->set_state(webrtc::MediaStreamTrackInterface::kEnded);
}
