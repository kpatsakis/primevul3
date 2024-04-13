  void StartHost() {
    DCHECK(context_->network_task_runner()->BelongsToCurrentThread());
    DCHECK(!host_);
    DCHECK(!signal_strategy_.get());

    if (shutting_down_)
      return;

    signal_strategy_.reset(
        new XmppSignalStrategy(context_->url_request_context_getter(),
                               xmpp_login_, xmpp_auth_token_,
                               xmpp_auth_service_));

    scoped_ptr<DnsBlackholeChecker> dns_blackhole_checker(
        new DnsBlackholeChecker(context_->url_request_context_getter(),
                                talkgadget_prefix_));

    signaling_connector_.reset(new SignalingConnector(
        signal_strategy_.get(),
        context_->url_request_context_getter(),
        dns_blackhole_checker.Pass(),
        base::Bind(&HostProcess::OnAuthFailed, base::Unretained(this))));

    if (!oauth_refresh_token_.empty()) {
      OAuthClientInfo client_info = {
        kUnofficialOAuth2ClientId,
        kUnofficialOAuth2ClientSecret
      };

#ifdef OFFICIAL_BUILD
      if (oauth_use_official_client_id_) {
        OAuthClientInfo official_client_info = {
            google_apis::GetOAuth2ClientID(google_apis::CLIENT_REMOTING),
            google_apis::GetOAuth2ClientSecret(google_apis::CLIENT_REMOTING)
        };

        client_info = official_client_info;
      }
#endif  // OFFICIAL_BUILD

      scoped_ptr<SignalingConnector::OAuthCredentials> oauth_credentials(
          new SignalingConnector::OAuthCredentials(
              xmpp_login_, oauth_refresh_token_, client_info));
      signaling_connector_->EnableOAuth(oauth_credentials.Pass());
    }

    NetworkSettings network_settings(
        allow_nat_traversal_ ?
        NetworkSettings::NAT_TRAVERSAL_ENABLED :
        NetworkSettings::NAT_TRAVERSAL_DISABLED);
    if (!allow_nat_traversal_) {
      network_settings.min_port = NetworkSettings::kDefaultMinPort;
      network_settings.max_port = NetworkSettings::kDefaultMaxPort;
    }

    host_ = new ChromotingHost(
        signal_strategy_.get(),
        desktop_environment_factory_.get(),
        CreateHostSessionManager(network_settings,
                                 context_->url_request_context_getter()),
        context_->capture_task_runner(),
        context_->encode_task_runner(),
        context_->network_task_runner());

#if defined(OS_LINUX)
    host_->SetMaximumSessionDuration(base::TimeDelta::FromHours(20));
#endif

    heartbeat_sender_.reset(new HeartbeatSender(
        this, host_id_, signal_strategy_.get(), &key_pair_));

    log_to_server_.reset(
        new LogToServer(host_, ServerLogEntry::ME2ME, signal_strategy_.get()));
    host_event_logger_ = HostEventLogger::Create(host_, kApplicationName);

    resizing_host_observer_.reset(
        new ResizingHostObserver(desktop_resizer_.get(), host_));

    curtaining_host_observer_.reset(new CurtainingHostObserver(
          curtain_.get(), host_));

    if (host_user_interface_.get()) {
      host_user_interface_->Start(
          host_, base::Bind(&HostProcess::OnDisconnectRequested,
                            base::Unretained(this)));
    }

    host_->Start(xmpp_login_);

    CreateAuthenticatorFactory();
  }
