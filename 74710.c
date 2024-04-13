void AutoFillHelper::FrameContentsAvailable(WebFrame* frame) {
  form_manager_.ExtractForms(frame);
  SendForms(frame);
}
