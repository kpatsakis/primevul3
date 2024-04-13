WebContentsImpl::WebContentsImpl(
    BrowserContext* browser_context,
    WebContentsImpl* opener)
    : delegate_(NULL),
      controller_(this, browser_context),
      render_view_host_delegate_view_(NULL),
      opener_(opener),
#if defined(OS_WIN) && defined(USE_AURA)
      accessible_parent_(NULL),
#endif
      render_manager_(this, this, this),
      is_loading_(false),
      crashed_status_(base::TERMINATION_STATUS_STILL_RUNNING),
      crashed_error_code_(0),
      waiting_for_response_(false),
      load_state_(net::LOAD_STATE_IDLE, string16()),
      upload_size_(0),
      upload_position_(0),
      displayed_insecure_content_(false),
      capturer_count_(0),
      should_normally_be_visible_(true),
      is_being_destroyed_(false),
      notify_disconnection_(false),
      dialog_manager_(NULL),
      is_showing_before_unload_dialog_(false),
      closed_by_user_gesture_(false),
      minimum_zoom_percent_(static_cast<int>(kMinimumZoomFactor * 100)),
      maximum_zoom_percent_(static_cast<int>(kMaximumZoomFactor * 100)),
      temporary_zoom_settings_(false),
      color_chooser_identifier_(0),
      message_source_(NULL),
      fullscreen_widget_routing_id_(MSG_ROUTING_NONE) {
  for (size_t i = 0; i < g_created_callbacks.Get().size(); i++)
    g_created_callbacks.Get().at(i).Run(this);
}
