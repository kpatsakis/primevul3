void WebContentsImpl::DidEndColorChooser() {
  Send(new ViewMsg_DidEndColorChooser(GetRoutingID(),
                                      color_chooser_identifier_));
  color_chooser_.reset();
  color_chooser_identifier_ = 0;
}
