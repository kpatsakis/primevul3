void FFmpegVideoDecodeEngine::ProduceVideoFrame(
    scoped_refptr<VideoFrame> frame) {
  DCHECK(frame.get() && !frame->IsEndOfStream());

  pending_output_buffers_++;

  frame_queue_available_.push_back(frame);

  if (flush_pending_) {
    TryToFinishPendingFlush();
  } else if (!output_eos_reached_) {
    ReadInput();
  }
}
