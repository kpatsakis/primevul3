void RenderThreadImpl::Init() {
  TRACE_EVENT_BEGIN_ETW("RenderThreadImpl::Init", 0, "");

  base::debug::TraceLog::GetInstance()->SetThreadSortIndex(
      base::PlatformThread::CurrentId(),
      kTraceEventRendererMainThreadSortIndex);

#if defined(OS_MACOSX) || defined(OS_ANDROID)
  blink::WebView::setUseExternalPopupMenus(true);
#endif

  lazy_tls.Pointer()->Set(this);

  ChildProcess::current()->set_main_thread(this);

  suspend_webkit_shared_timer_ = true;
  notify_webkit_of_modal_loop_ = true;
  webkit_shared_timer_suspended_ = false;
  widget_count_ = 0;
  hidden_widget_count_ = 0;
  idle_notification_delay_in_ms_ = kInitialIdleHandlerDelayMs;
  idle_notifications_to_skip_ = 0;
  layout_test_mode_ = false;

  appcache_dispatcher_.reset(
      new AppCacheDispatcher(Get(), new AppCacheFrontendImpl()));
  dom_storage_dispatcher_.reset(new DomStorageDispatcher());
  main_thread_indexed_db_dispatcher_.reset(new IndexedDBDispatcher(
      thread_safe_sender()));
  embedded_worker_dispatcher_.reset(new EmbeddedWorkerDispatcher());

  media_stream_center_ = NULL;

  db_message_filter_ = new DBMessageFilter();
  AddFilter(db_message_filter_.get());

  vc_manager_.reset(new VideoCaptureImplManager());
  AddFilter(vc_manager_->video_capture_message_filter());

#if defined(ENABLE_WEBRTC)
  peer_connection_tracker_.reset(new PeerConnectionTracker());
  AddObserver(peer_connection_tracker_.get());

  p2p_socket_dispatcher_ =
      new P2PSocketDispatcher(GetIOMessageLoopProxy().get());
  AddFilter(p2p_socket_dispatcher_.get());

  webrtc_identity_service_.reset(new WebRTCIdentityService());

  aec_dump_message_filter_ =
      new AecDumpMessageFilter(GetIOMessageLoopProxy(),
                               message_loop()->message_loop_proxy());
  AddFilter(aec_dump_message_filter_.get());

  peer_connection_factory_.reset(new PeerConnectionDependencyFactory(
      p2p_socket_dispatcher_.get()));
#endif  // defined(ENABLE_WEBRTC)

  audio_input_message_filter_ =
      new AudioInputMessageFilter(GetIOMessageLoopProxy());
  AddFilter(audio_input_message_filter_.get());

  audio_message_filter_ = new AudioMessageFilter(GetIOMessageLoopProxy());
  AddFilter(audio_message_filter_.get());

  midi_message_filter_ = new MidiMessageFilter(GetIOMessageLoopProxy());
  AddFilter(midi_message_filter_.get());

  AddFilter((new IndexedDBMessageFilter(thread_safe_sender()))->GetFilter());

  AddFilter((new EmbeddedWorkerContextMessageFilter())->GetFilter());

  GetContentClient()->renderer()->RenderThreadStarted();

  InitSkiaEventTracer();

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(cc::switches::kEnableGpuBenchmarking))
      RegisterExtension(GpuBenchmarkingExtension::Get());

  is_impl_side_painting_enabled_ =
      command_line.HasSwitch(switches::kEnableImplSidePainting);
  WebLayerImpl::SetImplSidePaintingEnabled(is_impl_side_painting_enabled_);

  is_zero_copy_enabled_ = command_line.HasSwitch(switches::kEnableZeroCopy) &&
                          !command_line.HasSwitch(switches::kDisableZeroCopy);

  is_one_copy_enabled_ = command_line.HasSwitch(switches::kEnableOneCopy);

  if (command_line.HasSwitch(switches::kDisableLCDText)) {
    is_lcd_text_enabled_ = false;
  } else if (command_line.HasSwitch(switches::kEnableLCDText)) {
    is_lcd_text_enabled_ = true;
  } else {
#if defined(OS_ANDROID)
    is_lcd_text_enabled_ = false;
#else
    is_lcd_text_enabled_ = true;
#endif
  }

  is_gpu_rasterization_enabled_ =
      command_line.HasSwitch(switches::kEnableGpuRasterization);
  is_gpu_rasterization_forced_ =
      command_line.HasSwitch(switches::kForceGpuRasterization);

  if (command_line.HasSwitch(switches::kDisableDistanceFieldText)) {
    is_distance_field_text_enabled_ = false;
  } else if (command_line.HasSwitch(switches::kEnableDistanceFieldText)) {
    is_distance_field_text_enabled_ = true;
  } else {
    is_distance_field_text_enabled_ = false;
  }

  is_low_res_tiling_enabled_ = true;
  if (command_line.HasSwitch(switches::kDisableLowResTiling) &&
      !command_line.HasSwitch(switches::kEnableLowResTiling)) {
    is_low_res_tiling_enabled_ = false;
  }

  base::FilePath media_path;
  PathService::Get(DIR_MEDIA_LIBS, &media_path);
  if (!media_path.empty())
    media::InitializeMediaLibrary(media_path);

  memory_pressure_listener_.reset(new base::MemoryPressureListener(
      base::Bind(&RenderThreadImpl::OnMemoryPressure, base::Unretained(this))));

  std::vector<base::DiscardableMemoryType> supported_types;
  base::DiscardableMemory::GetSupportedTypes(&supported_types);
  DCHECK(!supported_types.empty());

  base::DiscardableMemoryType type = supported_types[0];

  if (command_line.HasSwitch(switches::kUseDiscardableMemory)) {
    std::string requested_type_name = command_line.GetSwitchValueASCII(
        switches::kUseDiscardableMemory);
    base::DiscardableMemoryType requested_type =
        base::DiscardableMemory::GetNamedType(requested_type_name);
    if (std::find(supported_types.begin(),
                  supported_types.end(),
                  requested_type) != supported_types.end()) {
      type = requested_type;
    } else {
      LOG(ERROR) << "Requested discardable memory type is not supported.";
    }
  }

  base::DiscardableMemory::SetPreferredType(type);

  base::DiscardableMemory::RegisterMemoryPressureListeners();

  allocate_gpu_memory_buffer_thread_checker_.DetachFromThread();

  if (command_line.HasSwitch(switches::kNumRasterThreads)) {
    int num_raster_threads;
    std::string string_value =
        command_line.GetSwitchValueASCII(switches::kNumRasterThreads);
    if (base::StringToInt(string_value, &num_raster_threads) &&
        num_raster_threads >= kMinRasterThreads &&
        num_raster_threads <= kMaxRasterThreads) {
      cc::RasterWorkerPool::SetNumRasterThreads(num_raster_threads);
    } else {
      LOG(WARNING) << "Failed to parse switch " <<
                      switches::kNumRasterThreads  << ": " << string_value;
    }
  }

  service_registry()->AddService<RenderFrameSetup>(
      base::Bind(CreateRenderFrameSetup));

  TRACE_EVENT_END_ETW("RenderThreadImpl::Init", 0, "");
}
