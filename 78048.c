WebContentsImpl* WebContentsImpl::GetCreatedWindow(int route_id) {
  PendingContents::iterator iter = pending_contents_.find(route_id);

  if (iter == pending_contents_.end()) {
    return NULL;
  }

  WebContentsImpl* new_contents = iter->second;
  pending_contents_.erase(route_id);
  RemoveDestructionObserver(new_contents);

  if (new_contents->GetRenderProcessHost()->IsGuest())
    return new_contents;

  if (!new_contents->GetRenderProcessHost()->HasConnection() ||
      !new_contents->GetRenderViewHost()->GetView())
    return NULL;

  static_cast<RenderViewHostImpl*>(new_contents->GetRenderViewHost())->Init();
  return new_contents;
}
