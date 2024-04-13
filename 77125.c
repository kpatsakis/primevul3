static std::string CreateTrackLabel(
    const std::string& manager_label,
    int session_id,
    bool is_video) {
  std::string track_label = manager_label;
  if (is_video) {
    track_label += "#video-";
  } else {
    track_label += "#audio-";
  }
  track_label += session_id;
  return track_label;
}
