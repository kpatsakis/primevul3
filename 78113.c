void MakeNavigateParams(const NavigationEntryImpl& entry,
                        const NavigationControllerImpl& controller,
                        WebContentsDelegate* delegate,
                        NavigationController::ReloadType reload_type,
                        ViewMsg_Navigate_Params* params) {
  params->page_id = entry.GetPageID();
  params->should_clear_history_list = entry.should_clear_history_list();
  if (entry.should_clear_history_list()) {
    params->pending_history_list_offset = -1;
    params->current_history_list_offset = -1;
    params->current_history_list_length = 0;
  } else {
    params->pending_history_list_offset = controller.GetIndexOfEntry(&entry);
    params->current_history_list_offset =
        controller.GetLastCommittedEntryIndex();
    params->current_history_list_length = controller.GetEntryCount();
  }
  if (!entry.GetBaseURLForDataURL().is_empty()) {
    params->base_url_for_data_url = entry.GetBaseURLForDataURL();
    params->history_url_for_data_url = entry.GetVirtualURL();
  }
  params->referrer = entry.GetReferrer();
  params->transition = entry.GetTransitionType();
  params->page_state = entry.GetPageState();
  params->navigation_type =
      GetNavigationType(controller.GetBrowserContext(), entry, reload_type);
  params->request_time = base::Time::Now();
  params->extra_headers = entry.extra_headers();
  params->transferred_request_child_id =
      entry.transferred_global_request_id().child_id;
  params->transferred_request_request_id =
      entry.transferred_global_request_id().request_id;
  params->is_overriding_user_agent = entry.GetIsOverridingUserAgent();
  params->allow_download = !entry.IsViewSourceMode();
  params->is_post = entry.GetHasPostData();
  if(entry.GetBrowserInitiatedPostData()) {
      params->browser_initiated_post_data.assign(
          entry.GetBrowserInitiatedPostData()->front(),
          entry.GetBrowserInitiatedPostData()->front() +
              entry.GetBrowserInitiatedPostData()->size());

  }

  if (reload_type == NavigationControllerImpl::RELOAD_ORIGINAL_REQUEST_URL &&
      entry.GetOriginalRequestURL().is_valid() && !entry.GetHasPostData()) {
    params->url = entry.GetOriginalRequestURL();
  } else {
    params->url = entry.GetURL();
  }

  params->can_load_local_resources = entry.GetCanLoadLocalResources();
  params->frame_to_navigate = entry.GetFrameToNavigate();

  if (delegate)
    delegate->AddNavigationHeaders(params->url, &params->extra_headers);
}
