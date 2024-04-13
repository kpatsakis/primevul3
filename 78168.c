void WebContentsImpl::RouteCloseEvent(RenderViewHost* rvh) {
  if (rvh->GetSiteInstance()->IsRelatedSiteInstance(GetSiteInstance()))
    GetRenderViewHost()->ClosePage();
}
