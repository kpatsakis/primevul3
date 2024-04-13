int WebContentsImpl::CreateSwappedOutRenderView(
    SiteInstance* instance) {
  return render_manager_.CreateRenderView(instance, MSG_ROUTING_NONE,
                                          true, true);
}
