content::WebContents* DevToolsWindow::OpenURLFromTab(
    content::WebContents* source,
    const content::OpenURLParams& params) {
  if (!params.url.SchemeIs(chrome::kChromeDevToolsScheme)) {
    content::WebContents* inspected_web_contents = GetInspectedWebContents();
    return inspected_web_contents ?
        inspected_web_contents->OpenURL(params) : NULL;
  }

  content::DevToolsManager* manager = content::DevToolsManager::GetInstance();
  scoped_refptr<DevToolsAgentHost> agent_host(
      manager->GetDevToolsAgentHostFor(frontend_host_.get()));
  if (!agent_host.get())
    return NULL;
  manager->ClientHostClosing(frontend_host_.get());
  manager->RegisterDevToolsClientHostFor(agent_host.get(),
                                         frontend_host_.get());

  chrome::NavigateParams nav_params(profile_, params.url, params.transition);
  FillNavigateParamsFromOpenURLParams(&nav_params, params);
  nav_params.source_contents = source;
  nav_params.tabstrip_add_types = TabStripModel::ADD_NONE;
  nav_params.window_action = chrome::NavigateParams::SHOW_WINDOW;
  nav_params.user_gesture = params.user_gesture;
  chrome::Navigate(&nav_params);
  return nav_params.target_contents;
}
