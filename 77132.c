MediaStreamImpl::MediaStreamImpl(
    MediaStreamDispatcher* media_stream_dispatcher,
    content::P2PSocketDispatcher* p2p_socket_dispatcher,
    VideoCaptureImplManager* vc_manager,
    MediaStreamDependencyFactory* dependency_factory)
    : dependency_factory_(dependency_factory),
      media_stream_dispatcher_(media_stream_dispatcher),
      p2p_socket_dispatcher_(p2p_socket_dispatcher),
      network_manager_(NULL),
      vc_manager_(vc_manager),
      peer_connection_handler_(NULL),
      message_loop_proxy_(base::MessageLoopProxy::current()),
      signaling_thread_(NULL),
      worker_thread_(NULL),
      chrome_worker_thread_("Chrome_libJingle_WorkerThread") {
}
