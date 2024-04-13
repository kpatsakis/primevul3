FFmpegVideoDecodeEngine::FFmpegVideoDecodeEngine()
    : codec_context_(NULL),
      event_handler_(NULL),
      frame_rate_numerator_(0),
      frame_rate_denominator_(0),
      pending_input_buffers_(0),
      pending_output_buffers_(0),
      output_eos_reached_(false),
      flush_pending_(false) {
}
