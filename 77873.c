RenderFrameImpl::RenderFrameImpl(RenderViewImpl* render_view, int routing_id)
    : frame_(NULL),
      render_view_(render_view->AsWeakPtr()),
      routing_id_(routing_id),
      is_swapped_out_(false),
      is_detaching_(false),
      cookie_jar_(this),
      selection_text_offset_(0),
      selection_range_(gfx::Range::InvalidRange()),
      handling_select_range_(false),
      notification_provider_(NULL),
      media_stream_client_(NULL),
      web_user_media_client_(NULL),
      weak_factory_(this) {
  RenderThread::Get()->AddRoute(routing_id_, this);

#if defined(OS_ANDROID)
  new JavaBridgeDispatcher(this);
#endif

#if defined(ENABLE_NOTIFICATIONS)
  notification_provider_ = new NotificationProvider(this);
#endif
}
