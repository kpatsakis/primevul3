ResourceMessageFilter::ResourceMessageFilter(
    ResourceDispatcherHost* resource_dispatcher_host,
    int child_id,
    AudioRendererHost* audio_renderer_host,
    PluginService* plugin_service,
    printing::PrintJobManager* print_job_manager,
    Profile* profile,
    RenderWidgetHelper* render_widget_helper,
    URLRequestContextGetter* request_context)
    : Receiver(RENDER_PROCESS, child_id),
      channel_(NULL),
      resource_dispatcher_host_(resource_dispatcher_host),
      plugin_service_(plugin_service),
      print_job_manager_(print_job_manager),
      profile_(profile),
      ALLOW_THIS_IN_INITIALIZER_LIST(resolve_proxy_msg_helper_(this, NULL)),
      request_context_(request_context),
      media_request_context_(profile->GetRequestContextForMedia()),
      extensions_request_context_(profile->GetRequestContextForExtensions()),
      extensions_message_service_(profile->GetExtensionMessageService()),
      render_widget_helper_(render_widget_helper),
      audio_renderer_host_(audio_renderer_host),
      appcache_dispatcher_host_(
          new AppCacheDispatcherHost(profile->GetRequestContext())),
      ALLOW_THIS_IN_INITIALIZER_LIST(dom_storage_dispatcher_host_(
          new DOMStorageDispatcherHost(this, profile->GetWebKitContext(),
              resource_dispatcher_host->webkit_thread()))),
      ALLOW_THIS_IN_INITIALIZER_LIST(db_dispatcher_host_(
          new DatabaseDispatcherHost(profile->GetDatabaseTracker(), this,
              profile->GetHostContentSettingsMap()))),
      notification_prefs_(
          profile->GetDesktopNotificationService()->prefs_cache()),
      host_zoom_map_(profile->GetHostZoomMap()),
      off_the_record_(profile->IsOffTheRecord()),
      next_route_id_callback_(NewCallbackWithReturnValue(
          render_widget_helper, &RenderWidgetHelper::GetNextRoutingID)),
      ALLOW_THIS_IN_INITIALIZER_LIST(geolocation_dispatcher_host_(
          new GeolocationDispatcherHost(
              this->id(), new GeolocationPermissionContext(profile)))) {
  DCHECK(request_context_);
  DCHECK(media_request_context_);
  DCHECK(audio_renderer_host_.get());
  DCHECK(appcache_dispatcher_host_.get());
  DCHECK(dom_storage_dispatcher_host_.get());

  render_widget_helper_->Init(id(), resource_dispatcher_host_);
}
