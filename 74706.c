void AutoFillHelper::DidAcceptAutoFillSuggestion(const WebNode& node,
                                                 const WebString& value,
                                                 const WebString& label,
                                                 int unique_id,
                                                 unsigned index) {
  if (suggestions_options_index_ != -1 &&
      index == static_cast<unsigned>(suggestions_options_index_)) {
    render_view_->Send(new ViewHostMsg_ShowAutoFillDialog(
        render_view_->routing_id()));
  } else if (suggestions_clear_index_ != -1 &&
             index == static_cast<unsigned>(suggestions_clear_index_)) {
    form_manager_.ClearFormWithNode(node);
  } else if (form_manager_.FormWithNodeIsAutoFilled(node) || !unique_id) {
    WebInputElement element = node.toConst<WebInputElement>();

    element.setSuggestedValue(value);
    element.setValue(value);

    WebFrame* webframe = node.document().frame();
    if (webframe)
      webframe->notifiyPasswordListenerOfAutocomplete(element);
  } else {
    QueryAutoFillFormData(node, value, label, unique_id, AUTOFILL_FILL);
  }

  suggestions_clear_index_ = -1;
  suggestions_options_index_ = -1;
}
