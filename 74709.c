void AutoFillHelper::FormDataFilled(int query_id,
                                    const webkit_glue::FormData& form) {
  if (!render_view_->webview() || query_id != autofill_query_id_)
    return;

  switch (autofill_action_) {
    case AUTOFILL_FILL:
      form_manager_.FillForm(form, autofill_query_node_);
      break;
    case AUTOFILL_PREVIEW:
      form_manager_.PreviewForm(form);
      break;
    default:
      NOTREACHED();
  }
  autofill_action_ = AUTOFILL_NONE;
}
