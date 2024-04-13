DevToolsWindow* DevToolsWindow::Create(
    Profile* profile,
    const GURL& frontend_url,
    content::RenderViewHost* inspected_rvh,
    DevToolsDockSide dock_side,
    bool shared_worker_frontend,
    bool external_frontend) {
  GURL url(GetDevToolsURL(profile, frontend_url, dock_side,
                          shared_worker_frontend,
                          external_frontend));
  return new DevToolsWindow(profile, url, inspected_rvh, dock_side);
}
