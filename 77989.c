WebContents* WebContentsImpl::Clone() {
  CreateParams create_params(GetBrowserContext(), GetSiteInstance());
  create_params.initial_size = view_->GetContainerSize();
  WebContentsImpl* tc = CreateWithOpener(create_params, opener_);
  tc->GetController().CopyStateFrom(controller_);
  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    DidCloneToNewWebContents(this, tc));
  return tc;
}
