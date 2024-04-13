DevToolsWindow* DevToolsWindow::FindDevToolsWindow(
    DevToolsAgentHost* agent_host) {
  DevToolsWindows* instances = &g_instances.Get();
  content::DevToolsManager* manager = content::DevToolsManager::GetInstance();
  for (DevToolsWindows::iterator it(instances->begin()); it != instances->end();
       ++it) {
    if (manager->GetDevToolsAgentHostFor((*it)->frontend_host_.get()) ==
        agent_host)
      return *it;
  }
  return NULL;
}
