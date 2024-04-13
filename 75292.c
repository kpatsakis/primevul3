void PrepareFrameAndViewForPrint::FinishPrinting() {
  if (!finished_) {
    finished_ = true;
    frame_->printEnd();
    web_view_->resize(prev_view_size_);
    if (WebFrame* web_frame = web_view_->mainFrame())
      web_frame->setScrollOffset(prev_scroll_offset_);
  }
}
