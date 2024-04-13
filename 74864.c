bool RenderView::SendAndRunNestedMessageLoop(IPC::SyncMessage* message) {
  if (RenderThread::current())  // Will be NULL during unit tests.
    RenderThread::current()->DoNotNotifyWebKitOfModalLoop();

  message->EnableMessagePumping();  // Runs a nested message loop.
  return Send(message);
}
