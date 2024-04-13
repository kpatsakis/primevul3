void PageClickTracker::DidHandleMouseEvent(const WebMouseEvent& event) {
  if (event.type != WebInputEvent::MouseDown ||
      last_node_clicked_.isNull()) {
    return;
  }

  if (!last_node_clicked_.isElementNode())
    return;
  const WebElement& element = last_node_clicked_.toConst<WebElement>();
  if (!element.isFormControlElement())
    return;
  const WebFormControlElement& control =
      element.toConst<WebFormControlElement>();
  if (control.formControlType() != WebString::fromUTF8("text"))
    return;

  const WebInputElement& input_element = element.toConst<WebInputElement>();

  bool is_focused = (last_node_clicked_ == GetFocusedNode());
  ObserverListBase<PageClickListener>::Iterator it(listeners_);
  PageClickListener* listener;
  while ((listener = it.GetNext()) != NULL) {
    if (listener->InputElementClicked(input_element, was_focused_, is_focused))
      break;
  }

  last_node_clicked_.reset();
}
