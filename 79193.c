DevToolsWindow* DevToolsWindow::OpenDevToolsWindowForWorker(
    Profile* profile,
    DevToolsAgentHost* worker_agent) {
  DevToolsWindow* window = FindDevToolsWindow(worker_agent);
  if (!window) {
    window = DevToolsWindow::CreateDevToolsWindowForWorker(profile);
    content::DevToolsManager::GetInstance()->RegisterDevToolsClientHostFor(
        worker_agent, window->frontend_host_.get());
  }
  window->Show(DevToolsToggleAction::Show());
  return window;
}
