void RenderView::removeAutofillSuggestions(const WebString& name,
                                           const WebString& value) {
  autofill_helper_.RemoveAutocompleteSuggestion(name, value);
}
