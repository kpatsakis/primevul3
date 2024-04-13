void RenderView::BindDOMAutomationController(WebFrame* frame) {
  dom_automation_controller_.set_message_sender(this);
  dom_automation_controller_.set_routing_id(routing_id_);
  dom_automation_controller_.BindToJavascript(frame,
                                              L"domAutomationController");
}
