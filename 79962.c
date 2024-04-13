void TimeoutCallback(const std::string& timeout_message) {
  FAIL() << timeout_message;
  MessageLoop::current()->Quit();
}
