bool MediaStreamImpl::EnsurePeerConnectionFactory() {
  DCHECK(CalledOnValidThread());
  if (!signaling_thread_) {
    jingle_glue::JingleThreadWrapper::EnsureForCurrentThread();
    jingle_glue::JingleThreadWrapper::current()->set_send_allowed(true);
    signaling_thread_ = jingle_glue::JingleThreadWrapper::current();
  }

  if (!worker_thread_) {
    if (!chrome_worker_thread_.IsRunning()) {
      if (!chrome_worker_thread_.Start()) {
        LOG(ERROR) << "Could not start worker thread";
        signaling_thread_ = NULL;
        return false;
      }
    }
    base::WaitableEvent event(true, false);
    chrome_worker_thread_.message_loop()->PostTask(FROM_HERE, base::Bind(
        &MediaStreamImpl::InitializeWorkerThread,
        this,
        &worker_thread_,
        &event));
    event.Wait();
    DCHECK(worker_thread_);
  }

  if (!network_manager_)
    network_manager_ = new content::IpcNetworkManager(p2p_socket_dispatcher_);

  if (!socket_factory_.get()) {
    socket_factory_.reset(
        new content::IpcPacketSocketFactory(p2p_socket_dispatcher_));
  }

  if (!dependency_factory_->PeerConnectionFactoryCreated()) {
    if (!dependency_factory_->CreatePeerConnectionFactory(
            worker_thread_,
            signaling_thread_,
            p2p_socket_dispatcher_,
            network_manager_,
            socket_factory_.get())) {
      LOG(ERROR) << "Could not initialize PeerConnection factory";
      return false;
    }
  }

  return true;
}
