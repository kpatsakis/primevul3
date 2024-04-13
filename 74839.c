void RenderView::OnSetupDevToolsClient() {
  DCHECK(!devtools_client_.get());
  devtools_client_.reset(new DevToolsClient(this));
}
