void QuitMessageLoop(MessageLoop* message_loop) {
  message_loop->PostTask(FROM_HERE, MessageLoop::QuitClosure());
}
