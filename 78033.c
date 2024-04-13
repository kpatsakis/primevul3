WebContents* WebContents::FromRenderViewHost(const RenderViewHost* rvh) {
  return rvh->GetDelegate()->GetAsWebContents();
}
