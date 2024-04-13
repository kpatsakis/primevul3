WebUI* WebContentsImpl::GetWebUI() const {
  return render_manager_.web_ui() ? render_manager_.web_ui()
      : render_manager_.pending_web_ui();
}
