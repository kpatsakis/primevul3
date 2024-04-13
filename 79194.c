void DevToolsWindow::OpenExternalFrontend(
    Profile* profile,
    const std::string& frontend_url,
    content::DevToolsAgentHost* agent_host) {
  DevToolsWindow* window = FindDevToolsWindow(agent_host);
  if (!window) {
    window = Create(profile, DevToolsUI::GetProxyURL(frontend_url), NULL,
                    DEVTOOLS_DOCK_SIDE_UNDOCKED, false, true);
    content::DevToolsManager::GetInstance()->RegisterDevToolsClientHostFor(
        agent_host, window->frontend_host_.get());
  }
  window->Show(DevToolsToggleAction::Show());
}
