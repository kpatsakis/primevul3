void WebContentsImpl::SaveFrame(const GURL& url,
                                const Referrer& referrer) {
  if (!GetURL().is_valid())
    return;
  bool is_main_frame = (url == GetURL());

  DownloadManager* dlm =
      BrowserContext::GetDownloadManager(GetBrowserContext());
  if (!dlm)
    return;
  int64 post_id = -1;
  if (is_main_frame) {
    const NavigationEntry* entry = controller_.GetActiveEntry();
    if (entry)
      post_id = entry->GetPostID();
  }
  scoped_ptr<DownloadUrlParameters> params(
      DownloadUrlParameters::FromWebContents(this, url));
  params->set_referrer(referrer);
  params->set_post_id(post_id);
  params->set_prefer_cache(true);
  if (post_id >= 0)
    params->set_method("POST");
  params->set_prompt(true);
  dlm->DownloadUrl(params.Pass());
}
