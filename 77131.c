void MediaStreamImpl::InitializeWorkerThread(talk_base::Thread** thread,
                                             base::WaitableEvent* event) {
  jingle_glue::JingleThreadWrapper::EnsureForCurrentThread();
  jingle_glue::JingleThreadWrapper::current()->set_send_allowed(true);
  *thread = jingle_glue::JingleThreadWrapper::current();
  event->Signal();
}
