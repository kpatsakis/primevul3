  explicit HostProcess(scoped_ptr<ChromotingHostContext> context)
      : context_(context.Pass()),
        config_(FilePath()),
#ifdef OFFICIAL_BUILD
        oauth_use_official_client_id_(true),
#else
        oauth_use_official_client_id_(false),
#endif
        allow_nat_traversal_(true),
        restarting_(false),
        shutting_down_(false),
#if defined(OS_WIN)
        desktop_environment_factory_(new SessionDesktopEnvironmentFactory(
            context_->input_task_runner(), context_->ui_task_runner())),
#else  // !defined(OS_WIN)
        desktop_environment_factory_(new DesktopEnvironmentFactory(
            context_->input_task_runner(), context_->ui_task_runner())),
#endif  // !defined(OS_WIN)
        desktop_resizer_(DesktopResizer::Create()),
        exit_code_(kSuccessExitCode) {
    network_change_notifier_.reset(net::NetworkChangeNotifier::Create());
    curtain_ = CurtainMode::Create(
        base::Bind(&HostProcess::OnDisconnectRequested,
                   base::Unretained(this)),
        base::Bind(&HostProcess::OnDisconnectRequested,
                   base::Unretained(this)));
}
