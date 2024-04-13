void DevToolsWindow::FrontendWebContentsObserver::AboutToNavigateRenderView(
    content::RenderViewHost* render_view_host) {
  content::DevToolsClientHost::SetupDevToolsFrontendClient(render_view_host);
}
