void WebContentsImpl::DidChooseColorInColorChooser(SkColor color) {
  Send(new ViewMsg_DidChooseColorResponse(
      GetRoutingID(), color_chooser_identifier_, color));
}
