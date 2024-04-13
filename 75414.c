void FFmpegVideoDecodeEngine::Flush() {
  avcodec_flush_buffers(codec_context_);
  flush_pending_ = true;
  TryToFinishPendingFlush();
}
