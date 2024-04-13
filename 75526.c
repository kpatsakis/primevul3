void ExtensionService::InitEventRouters() {
  if (event_routers_initialized_)
    return;

  history_event_router_.reset(new ExtensionHistoryEventRouter());
  history_event_router_->ObserveProfile(profile_);
  ExtensionAccessibilityEventRouter::GetInstance()->ObserveProfile(profile_);
  browser_event_router_.reset(new ExtensionBrowserEventRouter(profile_));
  browser_event_router_->Init();
  preference_event_router_.reset(new ExtensionPreferenceEventRouter(profile_));
  bookmark_event_router_.reset(new ExtensionBookmarkEventRouter(
      profile_->GetBookmarkModel()));
  bookmark_event_router_->Init();
  cookies_event_router_.reset(new ExtensionCookiesEventRouter(profile_));
  cookies_event_router_->Init();
  management_event_router_.reset(new ExtensionManagementEventRouter(profile_));
  management_event_router_->Init();
  ExtensionProcessesEventRouter::GetInstance()->ObserveProfile(profile_);
  web_navigation_event_router_.reset(
      new ExtensionWebNavigationEventRouter(profile_));
  web_navigation_event_router_->Init();

#if defined(OS_CHROMEOS)
  file_browser_event_router_.reset(
      new ExtensionFileBrowserEventRouter(profile_));
  file_browser_event_router_->ObserveFileSystemEvents();
  chromeos::ExtensionInputMethodEventRouter::GetInstance();

  ExtensionMediaPlayerEventRouter::GetInstance()->Init(profile_);
  ExtensionInputImeEventRouter::GetInstance()->Init();
#endif

#if defined(OS_CHROMEOS) && defined(TOUCH_UI)
  ExtensionInputUiEventRouter::GetInstance()->Init();
#endif

  event_routers_initialized_ = true;
}
