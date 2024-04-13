void RenderView::unableToImplementPolicyWithError(
    WebFrame*, const WebURLError&) {
  NOTREACHED();  // Since we said we can handle all requests.
}
