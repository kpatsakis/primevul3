void WebContentsImpl::SetOverrideEncoding(const std::string& encoding) {
  SetEncoding(encoding);
  Send(new ViewMsg_SetPageEncoding(GetRoutingID(), encoding));
}
