void RenderView::runModal() {
  DCHECK(did_show_) << "should already have shown the view";

  if (RenderThread::current())  // Will be NULL during unit tests.
    RenderThread::current()->DoNotSuspendWebKitSharedTimer();

  SendAndRunNestedMessageLoop(new ViewHostMsg_RunModal(routing_id_));
}
