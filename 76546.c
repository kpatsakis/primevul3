void ChromotingInstance::Connect(const ClientConfig& config) {
  DCHECK(plugin_task_runner_->BelongsToCurrentThread());

  jingle_glue::JingleThreadWrapper::EnsureForCurrentMessageLoop();

  host_connection_.reset(new protocol::ConnectionToHost(true));
  scoped_ptr<AudioPlayer> audio_player(new PepperAudioPlayer(this));
  client_.reset(new ChromotingClient(config, &context_,
                                     host_connection_.get(), this,
                                     rectangle_decoder_.get(),
                                     audio_player.Pass()));

  mouse_input_filter_.set_input_stub(host_connection_->input_stub());
  mouse_input_filter_.set_input_size(view_->get_view_size_dips());

  LOG(INFO) << "Connecting to " << config.host_jid
            << ". Local jid: " << config.local_jid << ".";

  xmpp_proxy_ = new PepperXmppProxy(
      base::Bind(&ChromotingInstance::SendOutgoingIq, AsWeakPtr()),
      plugin_task_runner_, context_.main_task_runner());

  scoped_ptr<cricket::HttpPortAllocatorBase> port_allocator(
      PepperPortAllocator::Create(this));
  scoped_ptr<protocol::TransportFactory> transport_factory(
      new protocol::LibjingleTransportFactory(port_allocator.Pass(), false));

  client_->Start(xmpp_proxy_, transport_factory.Pass());

  plugin_task_runner_->PostDelayedTask(
      FROM_HERE, base::Bind(&ChromotingInstance::SendPerfStats, AsWeakPtr()),
      base::TimeDelta::FromMilliseconds(kPerfStatsIntervalMs));
}
