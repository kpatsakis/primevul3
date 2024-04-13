void FFmpegVideoDecodeEngine::ConsumeVideoSample(
    scoped_refptr<Buffer> buffer) {
  pending_input_buffers_--;
  if (flush_pending_) {
    TryToFinishPendingFlush();
  } else {
    DecodeFrame(buffer);
  }
}
