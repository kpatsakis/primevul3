  void OpenDevToolsWindowForSharedWorker(WorkerData* worker_data) {
    Profile* profile = browser()->profile();
    window_ = DevToolsWindow::CreateDevToolsWindowForWorker(profile);
    window_->Show(DevToolsToggleAction::Show());
    scoped_refptr<DevToolsAgentHost> agent_host(
        DevToolsAgentHost::GetForWorker(
            worker_data->worker_process_id,
            worker_data->worker_route_id));
    DevToolsManager::GetInstance()->RegisterDevToolsClientHostFor(
        agent_host.get(), window_->GetDevToolsClientHostForTest());
    RenderViewHost* client_rvh = window_->GetRenderViewHost();
    WebContents* client_contents = WebContents::FromRenderViewHost(client_rvh);
    if (client_contents->IsLoading()) {
      content::WindowedNotificationObserver observer(
          content::NOTIFICATION_LOAD_STOP,
          content::Source<NavigationController>(
              &client_contents->GetController()));
      observer.Wait();
    }
  }
