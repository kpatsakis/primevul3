void WebContentsImpl::CreateNewWindow(
    int route_id,
    int main_frame_route_id,
    const ViewHostMsg_CreateWindow_Params& params,
    SessionStorageNamespace* session_storage_namespace) {
  bool is_guest = GetRenderProcessHost()->IsGuest();

  scoped_refptr<SiteInstance> site_instance =
      params.opener_suppressed && !is_guest ?
      SiteInstance::CreateForURL(GetBrowserContext(), params.target_url) :
      GetSiteInstance();

  const std::string& partition_id =
      GetContentClient()->browser()->
          GetStoragePartitionIdForSite(GetBrowserContext(),
                                       site_instance->GetSiteURL());
  StoragePartition* partition = BrowserContext::GetStoragePartition(
      GetBrowserContext(), site_instance.get());
  DOMStorageContextWrapper* dom_storage_context =
      static_cast<DOMStorageContextWrapper*>(partition->GetDOMStorageContext());
  SessionStorageNamespaceImpl* session_storage_namespace_impl =
      static_cast<SessionStorageNamespaceImpl*>(session_storage_namespace);
  CHECK(session_storage_namespace_impl->IsFromContext(dom_storage_context));

  if (delegate_ &&
      !delegate_->ShouldCreateWebContents(this,
                                          route_id,
                                          params.window_container_type,
                                          params.frame_name,
                                          params.target_url,
                                          partition_id,
                                          session_storage_namespace)) {
    GetRenderViewHost()->GetProcess()->ResumeRequestsForView(route_id);
    GetRenderViewHost()->GetProcess()->ResumeRequestsForView(
        main_frame_route_id);
    return;
  }

  WebContentsImpl* new_contents =
      new WebContentsImpl(GetBrowserContext(),
                          params.opener_suppressed ? NULL : this);

  new_contents->GetController().SetSessionStorageNamespace(
      partition_id,
      session_storage_namespace);
  CreateParams create_params(GetBrowserContext(), site_instance.get());
  create_params.routing_id = route_id;
  create_params.main_frame_routing_id = main_frame_route_id;
  if (!is_guest) {
    create_params.context = view_->GetNativeView();
    create_params.initial_size = view_->GetContainerSize();
  } else {
    int instance_id = GetBrowserPluginGuestManager()->get_next_instance_id();
    WebContentsImpl* new_contents_impl =
        static_cast<WebContentsImpl*>(new_contents);
    BrowserPluginGuest::CreateWithOpener(instance_id, new_contents_impl,
        GetBrowserPluginGuest(), !!new_contents_impl->opener());
  }
  if (params.disposition == NEW_BACKGROUND_TAB)
    create_params.initially_hidden = true;
  new_contents->Init(create_params);

  if (!params.opener_suppressed) {
    if (!is_guest) {
      WebContentsViewPort* new_view = new_contents->view_.get();

      new_view->CreateViewForWidget(new_contents->GetRenderViewHost());
    }
    DCHECK_NE(MSG_ROUTING_NONE, route_id);
    pending_contents_[route_id] = new_contents;
    AddDestructionObserver(new_contents);
  }

  if (delegate_) {
    delegate_->WebContentsCreated(
        this, params.opener_frame_id, params.frame_name,
        params.target_url, new_contents);
  }

  if (params.opener_suppressed) {
    bool was_blocked = false;
    if (delegate_) {
      gfx::Rect initial_pos;
      delegate_->AddNewContents(
          this, new_contents, params.disposition, initial_pos,
          params.user_gesture, &was_blocked);
    }
    if (!was_blocked) {
      OpenURLParams open_params(params.target_url,
                                Referrer(),
                                CURRENT_TAB,
                                PAGE_TRANSITION_LINK,
                                true /* is_renderer_initiated */);
      open_params.user_gesture = params.user_gesture;
      new_contents->OpenURL(open_params);
    }
  }
}
