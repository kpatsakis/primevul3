void WebContentsImpl::DidNavigate(
    RenderViewHost* rvh,
    const ViewHostMsg_FrameNavigate_Params& params) {
  if (!frame_tree_root_.get()) {
    DCHECK(PageTransitionIsMainFrame(params.transition));
    frame_tree_root_.reset(new FrameTreeNode(params.frame_id, std::string()));
  }

  if (PageTransitionIsMainFrame(params.transition)) {
    if (delegate_ && delegate_->CanOverscrollContent())
      controller_.TakeScreenshot();

    render_manager_.DidNavigateMainFrame(rvh);
  }

  DCHECK(frame_tree_root_.get());

  if (!static_cast<SiteInstanceImpl*>(GetSiteInstance())->HasSite() &&
      ShouldAssignSiteForURL(params.url)) {
    static_cast<SiteInstanceImpl*>(GetSiteInstance())->SetSite(params.url);
  }

  if (PageTransitionIsMainFrame(params.transition))
    contents_mime_type_ = params.contents_mime_type;

  LoadCommittedDetails details;
  bool did_navigate = controller_.RendererDidNavigate(params, &details);

  FrameTreeNode* node = FindFrameTreeNodeByID(params.frame_id);
  if (node)
    node->set_current_url(params.url);

  if (details.type != NAVIGATION_TYPE_NAV_IGNORE) {
    bool is_main_frame = did_navigate ? details.is_main_frame : false;
    PageTransition transition_type = params.transition;
    if (did_navigate &&
        (controller_.GetActiveEntry()->GetTransitionType() &
            PAGE_TRANSITION_FORWARD_BACK)) {
      transition_type = PageTransitionFromInt(
          params.transition | PAGE_TRANSITION_FORWARD_BACK);
    }
    FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                      DidCommitProvisionalLoadForFrame(params.frame_id,
                      is_main_frame, params.url, transition_type, rvh));
  }

  if (!did_navigate)
    return;  // No navigation happened.


  if (details.is_main_frame) {
    DidNavigateMainFramePostCommit(details, params);
    if (delegate_) {
      delegate_->DidNavigateMainFramePostCommit(this);
      view_->SetOverscrollControllerEnabled(delegate_->CanOverscrollContent());
    }
  }
  DidNavigateAnyFramePostCommit(rvh, details, params);
}
