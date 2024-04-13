void PageClickTracker::StartTrackingFrame(WebFrame* frame) {
  tracked_frames_.push_back(frame);
  frame->document().addEventListener("mousedown", this, false);
}
