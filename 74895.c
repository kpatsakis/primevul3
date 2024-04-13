void RenderView::didAcceptAutoFillSuggestion(const WebKit::WebNode& node,
                                             const WebKit::WebString& value,
                                             const WebKit::WebString& label,
                                             int unique_id,
                                             unsigned index) {
  autofill_helper_.DidAcceptAutoFillSuggestion(node, value, label, unique_id,
                                               index);
}
