void WebContentsImpl::NotifySwappedFromRenderManager(RenderViewHost* rvh) {
  NotifySwapped(rvh);

  if (delegate_)
    view_->SetOverscrollControllerEnabled(delegate_->CanOverscrollContent());

  view_->RenderViewSwappedIn(render_manager_.current_host());

  FrameTreeNode* root = NULL;
  RenderViewHostImpl* new_rvh = static_cast<RenderViewHostImpl*>(
      render_manager_.current_host());

  if (new_rvh->main_frame_id() != -1)
    root = new FrameTreeNode(new_rvh->main_frame_id(), std::string());

  frame_tree_root_.reset(root);
}
