void WebContentsImpl::OnDidRunInsecureContent(
    const std::string& security_origin, const GURL& target_url) {
  LOG(INFO) << security_origin << " ran insecure content from "
            << target_url.possibly_invalid_spec();
  RecordAction(UserMetricsAction("SSL.RanInsecureContent"));
  if (EndsWith(security_origin, kDotGoogleDotCom, false))
    RecordAction(UserMetricsAction("SSL.RanInsecureContentGoogle"));
  controller_.ssl_manager()->DidRunInsecureContent(security_origin);
  displayed_insecure_content_ = true;
  SSLManager::NotifySSLInternalStateChanged(
      GetController().GetBrowserContext());
}
