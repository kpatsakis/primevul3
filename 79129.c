void DevToolsWindowBeforeUnloadObserver::Wait() {
  if (m_fired)
    return;
  message_loop_runner_ = new content::MessageLoopRunner;
  message_loop_runner_->Run();
}
