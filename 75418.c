void FFmpegVideoDecodeEngine::TryToFinishPendingFlush() {
  DCHECK(flush_pending_);

  if (!pending_input_buffers_ && !pending_output_buffers_) {
    flush_pending_ = false;
    event_handler_->OnFlushComplete();
  }
}
