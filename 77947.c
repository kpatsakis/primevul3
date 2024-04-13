void RenderFrameImpl::willRequestAfterPreconnect(
    blink::WebLocalFrame* frame,
    blink::WebURLRequest& request) {
  DCHECK(!frame_ || frame_ == frame);
  WebString custom_user_agent;

  DCHECK(!request.extraData());

  bool was_after_preconnect_request = true;
  RequestExtraData* extra_data = new RequestExtraData();
  extra_data->set_custom_user_agent(custom_user_agent);
  extra_data->set_was_after_preconnect_request(was_after_preconnect_request);
  request.setExtraData(extra_data);
}
