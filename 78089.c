WebPreferences WebContentsImpl::GetWebkitPrefs(RenderViewHost* rvh,
                                               const GURL& url) {
  TRACE_EVENT0("browser", "WebContentsImpl::GetWebkitPrefs");
  WebPreferences prefs;

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();

  prefs.javascript_enabled =
      !command_line.HasSwitch(switches::kDisableJavaScript);
  prefs.web_security_enabled =
      !command_line.HasSwitch(switches::kDisableWebSecurity);
  prefs.plugins_enabled =
      !command_line.HasSwitch(switches::kDisablePlugins);
  prefs.java_enabled =
      !command_line.HasSwitch(switches::kDisableJava);

  prefs.remote_fonts_enabled =
      !command_line.HasSwitch(switches::kDisableRemoteFonts);
  prefs.xss_auditor_enabled =
      !command_line.HasSwitch(switches::kDisableXSSAuditor);
  prefs.application_cache_enabled =
      !command_line.HasSwitch(switches::kDisableApplicationCache);

  prefs.local_storage_enabled =
      !command_line.HasSwitch(switches::kDisableLocalStorage);
  prefs.databases_enabled =
      !command_line.HasSwitch(switches::kDisableDatabases);
  prefs.webaudio_enabled =
      !command_line.HasSwitch(switches::kDisableWebAudio);

  prefs.experimental_webgl_enabled =
      GpuProcessHost::gpu_enabled() &&
      !command_line.HasSwitch(switches::kDisable3DAPIs) &&
      !command_line.HasSwitch(switches::kDisableExperimentalWebGL);

  prefs.flash_3d_enabled =
      GpuProcessHost::gpu_enabled() &&
      !command_line.HasSwitch(switches::kDisableFlash3d);
  prefs.flash_stage3d_enabled =
      GpuProcessHost::gpu_enabled() &&
      !command_line.HasSwitch(switches::kDisableFlashStage3d);
  prefs.flash_stage3d_baseline_enabled =
      GpuProcessHost::gpu_enabled() &&
      !command_line.HasSwitch(switches::kDisableFlashStage3d);

  prefs.gl_multisampling_enabled =
      !command_line.HasSwitch(switches::kDisableGLMultisampling);
  prefs.privileged_webgl_extensions_enabled =
      command_line.HasSwitch(switches::kEnablePrivilegedWebGLExtensions);
  prefs.site_specific_quirks_enabled =
      !command_line.HasSwitch(switches::kDisableSiteSpecificQuirks);
  prefs.allow_file_access_from_file_urls =
      command_line.HasSwitch(switches::kAllowFileAccessFromFiles);

  prefs.accelerated_compositing_for_overflow_scroll_enabled = false;
  if (command_line.HasSwitch(switches::kEnableAcceleratedOverflowScroll))
    prefs.accelerated_compositing_for_overflow_scroll_enabled = true;
  if (command_line.HasSwitch(switches::kDisableAcceleratedOverflowScroll))
    prefs.accelerated_compositing_for_overflow_scroll_enabled = false;

  prefs.accelerated_compositing_for_scrollable_frames_enabled =
      command_line.HasSwitch(switches::kEnableAcceleratedScrollableFrames);
  prefs.composited_scrolling_for_frames_enabled =
      command_line.HasSwitch(switches::kEnableCompositedScrollingForFrames);
  prefs.show_paint_rects =
      command_line.HasSwitch(switches::kShowPaintRects);
  prefs.accelerated_compositing_enabled =
      GpuProcessHost::gpu_enabled() &&
      !command_line.HasSwitch(switches::kDisableAcceleratedCompositing);
  prefs.force_compositing_mode =
      content::IsForceCompositingModeEnabled() &&
      !command_line.HasSwitch(switches::kDisableForceCompositingMode);
  prefs.accelerated_2d_canvas_enabled =
      GpuProcessHost::gpu_enabled() &&
      !command_line.HasSwitch(switches::kDisableAccelerated2dCanvas);
  prefs.antialiased_2d_canvas_disabled =
      command_line.HasSwitch(switches::kDisable2dCanvasAntialiasing);
  prefs.accelerated_filters_enabled =
      GpuProcessHost::gpu_enabled() &&
      command_line.HasSwitch(switches::kEnableAcceleratedFilters);
  prefs.accelerated_compositing_for_3d_transforms_enabled =
      prefs.accelerated_compositing_for_animation_enabled =
          !command_line.HasSwitch(switches::kDisableAcceleratedLayers);
  prefs.accelerated_compositing_for_plugins_enabled =
      !command_line.HasSwitch(switches::kDisableAcceleratedPlugins);
  prefs.accelerated_compositing_for_video_enabled =
      !command_line.HasSwitch(switches::kDisableAcceleratedVideo);
  prefs.fullscreen_enabled =
      !command_line.HasSwitch(switches::kDisableFullScreen);
  prefs.css_sticky_position_enabled =
      command_line.HasSwitch(switches::kEnableExperimentalWebPlatformFeatures);
  prefs.css_shaders_enabled =
      command_line.HasSwitch(switches::kEnableCssShaders);
  prefs.lazy_layout_enabled =
      command_line.HasSwitch(switches::kEnableExperimentalWebPlatformFeatures);
  prefs.region_based_columns_enabled =
      command_line.HasSwitch(switches::kEnableRegionBasedColumns);
  prefs.threaded_html_parser =
      !command_line.HasSwitch(switches::kDisableThreadedHTMLParser);
  prefs.experimental_websocket_enabled =
      command_line.HasSwitch(switches::kEnableExperimentalWebSocket);
  if (command_line.HasSwitch(cc::switches::kEnablePinchVirtualViewport)) {
    prefs.pinch_virtual_viewport_enabled = true;
    prefs.pinch_overlay_scrollbar_thickness = 10;
  }

#if defined(OS_ANDROID)
  prefs.use_solid_color_scrollbars = true;
  prefs.user_gesture_required_for_media_playback = !command_line.HasSwitch(
      switches::kDisableGestureRequirementForMediaPlayback);
  prefs.user_gesture_required_for_media_fullscreen = !command_line.HasSwitch(
      switches::kDisableGestureRequirementForMediaFullscreen);
#endif

  prefs.touch_enabled = ui::AreTouchEventsEnabled();
  prefs.device_supports_touch = prefs.touch_enabled &&
      ui::IsTouchDevicePresent();
#if defined(OS_ANDROID)
  prefs.device_supports_mouse = false;
#endif

   prefs.touch_adjustment_enabled =
       !command_line.HasSwitch(switches::kDisableTouchAdjustment);

#if defined(OS_MACOSX) || defined(OS_CHROMEOS)
  bool default_enable_scroll_animator = true;
#else
  bool default_enable_scroll_animator = false;
#endif
  prefs.enable_scroll_animator = default_enable_scroll_animator;
  if (command_line.HasSwitch(switches::kEnableSmoothScrolling))
    prefs.enable_scroll_animator = true;
  if (command_line.HasSwitch(switches::kDisableSmoothScrolling))
    prefs.enable_scroll_animator = false;

  prefs.visual_word_movement_enabled =
      command_line.HasSwitch(switches::kEnableVisualWordMovement);

  GpuDataManagerImpl::GetInstance()->UpdateRendererWebPrefs(&prefs);

  if (ChildProcessSecurityPolicyImpl::GetInstance()->HasWebUIBindings(
          rvh->GetProcess()->GetID())) {
    prefs.loads_images_automatically = true;
    prefs.javascript_enabled = true;
  }

  prefs.is_online = !net::NetworkChangeNotifier::IsOffline();

#if !defined(USE_AURA)
  if ((url.SchemeIs(chrome::kChromeUIScheme) ||
      (url.SchemeIs(chrome::kAboutScheme) &&
       url.spec() != kAboutBlankURL)) &&
      !command_line.HasSwitch(switches::kAllowWebUICompositing)) {
    prefs.accelerated_compositing_enabled = false;
    prefs.accelerated_2d_canvas_enabled = false;
  }
#endif

  prefs.fixed_position_creates_stacking_context = !command_line.HasSwitch(
      switches::kDisableFixedPositionCreatesStackingContext);

#if defined(OS_CHROMEOS)
  prefs.gesture_tap_highlight_enabled = !command_line.HasSwitch(
      switches::kDisableGestureTapHighlight);
#else
  prefs.gesture_tap_highlight_enabled = command_line.HasSwitch(
      switches::kEnableGestureTapHighlight);
#endif

  prefs.number_of_cpu_cores = base::SysInfo::NumberOfProcessors();

  prefs.viewport_enabled = command_line.HasSwitch(switches::kEnableViewport);

  prefs.deferred_image_decoding_enabled =
      command_line.HasSwitch(switches::kEnableDeferredImageDecoding) ||
      cc::switches::IsImplSidePaintingEnabled();

  prefs.spatial_navigation_enabled = command_line.HasSwitch(
      switches::kEnableSpatialNavigation);

  GetContentClient()->browser()->OverrideWebkitPrefs(rvh, url, &prefs);

  bool guest_compositing_enabled = !command_line.HasSwitch(
      switches::kDisableBrowserPluginCompositing);
  if (rvh->GetProcess()->IsGuest() && !guest_compositing_enabled) {
    prefs.force_compositing_mode = false;
    prefs.accelerated_compositing_enabled = false;
  }

  return prefs;
}
