void PageClickTracker::StopTrackingFrame(WebFrame* frame, bool frame_detached) {
  FrameList::iterator iter =
      std::find(tracked_frames_.begin(), tracked_frames_.end(), frame);
  if (iter == tracked_frames_.end()) {
    return;
  }
  tracked_frames_.erase(iter);
  if (!frame_detached)
    frame->document().removeEventListener("mousedown", this, false);
}
