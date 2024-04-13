WebKit::WebSpeechInputController* RenderView::speechInputController(
    WebKit::WebSpeechInputListener* listener) {
  if (!speech_input_dispatcher_.get())
    speech_input_dispatcher_.reset(new SpeechInputDispatcher(this, listener));
  return speech_input_dispatcher_.get();
}
