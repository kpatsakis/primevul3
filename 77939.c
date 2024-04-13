bool RenderFrameImpl::shouldReportDetailedMessageForSource(
    const blink::WebString& source) {
  return GetContentClient()->renderer()->ShouldReportDetailedMessageForSource(
      source);
}
