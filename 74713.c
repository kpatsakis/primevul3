void AutoFillHelper::RemoveAutocompleteSuggestion(
    const WebKit::WebString& name, const WebKit::WebString& value) {
  if (suggestions_clear_index_ != -1)
    suggestions_clear_index_--;
  if (suggestions_options_index_ != -1)
    suggestions_options_index_--;

  render_view_->Send(new ViewHostMsg_RemoveAutocompleteEntry(
      render_view_->routing_id(), name, value));
}
