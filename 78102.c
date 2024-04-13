void WebContentsImpl::Init(const WebContents::CreateParams& params) {
  render_manager_.Init(
      params.browser_context, params.site_instance, params.routing_id,
      params.main_frame_routing_id);

  view_.reset(GetContentClient()->browser()->
      OverrideCreateWebContentsView(this, &render_view_host_delegate_view_));
  if (view_) {
    CHECK(render_view_host_delegate_view_);
  } else {
    WebContentsViewDelegate* delegate =
        GetContentClient()->browser()->GetWebContentsViewDelegate(this);

    if (browser_plugin_guest_) {
      scoped_ptr<WebContentsViewPort> platform_view(CreateWebContentsView(
          this, delegate, &render_view_host_delegate_view_));

      WebContentsViewGuest* rv = new WebContentsViewGuest(
          this, browser_plugin_guest_.get(), platform_view.Pass(),
          render_view_host_delegate_view_);
      render_view_host_delegate_view_ = rv;
      view_.reset(rv);
    } else {
      view_.reset(CreateWebContentsView(
          this, delegate, &render_view_host_delegate_view_));
    }
    CHECK(render_view_host_delegate_view_);
  }
  CHECK(view_.get());

  gfx::Size initial_size = params.initial_size;
  view_->CreateView(initial_size, params.context);

  if (opener_)
    AddDestructionObserver(opener_);

  registrar_.Add(this,
                 NOTIFICATION_RENDER_WIDGET_HOST_DESTROYED,
                 NotificationService::AllBrowserContextsAndSources());
#if defined(OS_ANDROID)
  java_bridge_dispatcher_host_manager_.reset(
      new JavaBridgeDispatcherHostManager(this));
#endif

#if defined(OS_ANDROID)
  date_time_chooser_.reset(new DateTimeChooserAndroid());
#endif
}
