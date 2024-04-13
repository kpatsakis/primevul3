void AutoFillHelper::QueryAutocompleteSuggestions(const WebNode& node,
                                                  const WebString& name,
                                                  const WebString& value) {
  static int query_counter = 0;
  autofill_query_id_ = query_counter++;
  autofill_query_node_ = node;

  const WebFormControlElement& element = node.toConst<WebFormControlElement>();
  webkit_glue::FormField field;
  FormManager::WebFormControlElementToFormField(element, true, &field);

  field.set_label(FormManager::LabelForElement(element));

  bool form_autofilled = form_manager_.FormWithNodeIsAutoFilled(node);
  render_view_->Send(new ViewHostMsg_QueryFormFieldAutoFill(
      render_view_->routing_id(), autofill_query_id_, form_autofilled, field));
}
