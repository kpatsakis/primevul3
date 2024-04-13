void RenderView::queryAutofillSuggestions(const WebNode& node,
                                          const WebString& name,
                                          const WebString& value) {
  autofill_helper_.QueryAutocompleteSuggestions(node, name, value);
}
