void FFmpegVideoDecodeEngine::ReadInput() {
  DCHECK_EQ(output_eos_reached_, false);
  pending_input_buffers_++;
  event_handler_->ProduceVideoSample(NULL);
}
