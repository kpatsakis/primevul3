bool WebContentsImpl::ShouldAssignSiteForURL(const GURL& url) {
  if (url == GURL(kAboutBlankURL))
    return false;

  return GetContentClient()->browser()->ShouldAssignSiteForURL(url);
}
