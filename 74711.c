 void AutoFillHelper::QueryAutoFillFormData(const WebNode& node,
                                            const WebString& value,
                                            const WebString& label,
                                           int unique_id,
                                           AutoFillAction action) {
  static int query_counter = 0;
  autofill_query_id_ = query_counter++;

  webkit_glue::FormData form;
  const WebInputElement element = node.toConst<WebInputElement>();
  if (!form_manager_.FindFormWithFormControlElement(
          element, FormManager::REQUIRE_NONE, &form))
    return;

  autofill_action_ = action;
  render_view_->Send(new ViewHostMsg_FillAutoFillFormData(
      render_view_->routing_id(), autofill_query_id_, form, value, label,
      unique_id));
}
