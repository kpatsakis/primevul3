DevToolsWindow* DevToolsWindow::ToggleDevToolsWindow(
    content::RenderViewHost* inspected_rvh,
    bool force_open,
    const DevToolsToggleAction& action) {
  scoped_refptr<DevToolsAgentHost> agent(
      DevToolsAgentHost::GetOrCreateFor(inspected_rvh));
  content::DevToolsManager* manager = content::DevToolsManager::GetInstance();
  DevToolsWindow* window = FindDevToolsWindow(agent.get());
  bool do_open = force_open;
  if (!window) {
    Profile* profile = Profile::FromBrowserContext(
        inspected_rvh->GetProcess()->GetBrowserContext());
    DevToolsDockSide dock_side = GetDockSideFromPrefs(profile);
    content::RecordAction(
        content::UserMetricsAction("DevTools_InspectRenderer"));
    window = Create(profile, GURL(), inspected_rvh, dock_side, false, false);
    manager->RegisterDevToolsClientHostFor(agent.get(),
                                           window->frontend_host_.get());
    do_open = true;
  }

  window->UpdateBrowserToolbar();

  if (window->dock_side_ == DEVTOOLS_DOCK_SIDE_MINIMIZED)
    window->Restore();
  else if (!window->IsDocked() || do_open)
    window->Show(action);
  else
    window->CloseWindow();

  return window;
}
