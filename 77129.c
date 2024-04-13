webrtc::MediaStreamTrackInterface* MediaStreamImpl::GetLocalMediaStreamTrack(
    const std::string& label) {
  DCHECK(CalledOnValidThread());
  MediaStreamTrackPtrMap::iterator it = local_tracks_.find(label);
  if (it == local_tracks_.end())
    return NULL;
  MediaStreamTrackPtr stream = it->second;
  return stream.get();
}
