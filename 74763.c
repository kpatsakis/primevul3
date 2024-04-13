void RenderView::Init(gfx::NativeViewId parent_hwnd,
                      int32 opener_id,
                      const RendererPreferences& renderer_prefs,
                      SharedRenderViewCounter* counter,
                      int32 routing_id,
                      const string16& frame_name) {
  DCHECK(!webview());

  if (opener_id != MSG_ROUTING_NONE)
    opener_id_ = opener_id;

  if (counter) {
    shared_popup_counter_ = counter;
    shared_popup_counter_->data++;
    decrement_shared_popup_at_destruction_ = true;
  } else {
    shared_popup_counter_ = new SharedRenderViewCounter(0);
    decrement_shared_popup_at_destruction_ = false;
  }

  devtools_agent_.reset(new DevToolsAgent(routing_id, this));

  webwidget_ = WebView::create(this, devtools_agent_.get());
  Singleton<ViewMap>::get()->insert(std::make_pair(webview(), this));
  webkit_preferences_.Apply(webview());
  webview()->initializeMainFrame(this);
  if (!frame_name.empty())
    webview()->mainFrame()->setName(frame_name);

  OnSetRendererPrefs(renderer_prefs);

  routing_id_ = routing_id;
  render_thread_->AddRoute(routing_id_, this);
  AddRef();

  if (opener_id == MSG_ROUTING_NONE) {
    did_show_ = true;
    CompleteInit(parent_hwnd);
  }

  host_window_ = parent_hwnd;

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kDomAutomationController))
    enabled_bindings_ |= BindingsPolicy::DOM_AUTOMATION;

  audio_message_filter_ = new AudioMessageFilter(routing_id_);
  render_thread_->AddFilter(audio_message_filter_);
}
