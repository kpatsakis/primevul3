bool WebURLLoaderImpl::Context::CanHandleDataURL(const GURL& url) const {
  DCHECK(url.SchemeIs("data"));


#if defined(OS_ANDROID)
  if (request_.targetType() == WebURLRequest::TargetIsMainFrame)
    return false;
#endif

  if (request_.targetType() != WebURLRequest::TargetIsMainFrame &&
      request_.targetType() != WebURLRequest::TargetIsSubframe)
    return true;

  std::string mime_type, unused_charset;
  if (net::DataURL::Parse(url, &mime_type, &unused_charset, NULL) &&
      net::IsSupportedMimeType(mime_type))
    return true;

  return false;
}
