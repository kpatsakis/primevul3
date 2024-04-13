void RenderThreadImpl::UpdateHistograms(int sequence_number) {
  child_histogram_message_filter()->SendHistograms(sequence_number);
}
