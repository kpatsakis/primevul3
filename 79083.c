WebURLError WebURLLoaderImpl::CreateError(const WebURL& unreachable_url,
                                          int reason) {
  WebURLError error;
  error.domain = WebString::fromUTF8(net::kErrorDomain);
  error.reason = reason;
  error.unreachableURL = unreachable_url;
  if (reason == net::ERR_ABORTED) {
    error.isCancellation = true;
  } else if (reason == net::ERR_TEMPORARILY_THROTTLED) {
    error.localizedDescription = WebString::fromUTF8(
        kThrottledErrorDescription);
  }
  return error;
}
