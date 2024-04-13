void AutoFillHelper::SendForms(WebFrame* frame) {
  WebKit::WebVector<WebFormElement> web_forms;
  frame->forms(web_forms);

  std::vector<webkit_glue::FormData> forms;
  for (size_t i = 0; i < web_forms.size(); ++i) {
    const WebFormElement& web_form = web_forms[i];

    webkit_glue::FormData form;
    if (FormManager::WebFormElementToFormData(
            web_form, FormManager::REQUIRE_NONE, false, &form)) {
      forms.push_back(form);
    }
  }

  if (!forms.empty()) {
    render_view_->Send(new ViewHostMsg_FormsSeen(render_view_->routing_id(),
                                                 forms));
  }
}
