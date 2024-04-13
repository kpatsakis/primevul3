bool WebContentsImpl::NavigateToEntry(
    const NavigationEntryImpl& entry,
    NavigationController::ReloadType reload_type) {
  TRACE_EVENT0("browser", "WebContentsImpl::NavigateToEntry");

  if (entry.GetURL().spec().size() > kMaxURLChars) {
    LOG(WARNING) << "Refusing to load URL as it exceeds " << kMaxURLChars
                 << " characters.";
    return false;
  }

  RenderViewHostImpl* dest_render_view_host =
      static_cast<RenderViewHostImpl*>(render_manager_.Navigate(entry));
  if (!dest_render_view_host)
    return false;  // Unable to create the desired render view host.

  int enabled_bindings = dest_render_view_host->GetEnabledBindings();
  bool data_urls_allowed = delegate_ && delegate_->CanLoadDataURLsInWebUI();
  bool is_allowed_in_web_ui_renderer =
      WebUIControllerFactoryRegistry::GetInstance()->IsURLAcceptableForWebUI(
          GetBrowserContext(), entry.GetURL(), data_urls_allowed);
  if ((enabled_bindings & BINDINGS_POLICY_WEB_UI) &&
      !is_allowed_in_web_ui_renderer) {
    GetContentClient()->SetActiveURL(entry.GetURL());
    CHECK(0);
  }

  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    AboutToNavigateRenderView(dest_render_view_host));

  current_load_start_ = base::TimeTicks::Now();

  ViewMsg_Navigate_Params navigate_params;
  MakeNavigateParams(entry, controller_, delegate_, reload_type,
                     &navigate_params);
  dest_render_view_host->Navigate(navigate_params);

  if (entry.GetPageID() == -1) {
    if (entry.GetURL().SchemeIs(kJavaScriptScheme))
      return false;
  }

  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    NavigateToPendingEntry(entry.GetURL(), reload_type));

  if (delegate_)
    delegate_->DidNavigateToPendingEntry(this);

  return true;
}
