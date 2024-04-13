void FFmpegVideoDecodeEngine::Seek() {
  output_eos_reached_ = false;

  for (unsigned int i = 0; i < Limits::kMaxVideoFrames; ++i)
    ReadInput();

  event_handler_->OnSeekComplete();
}
