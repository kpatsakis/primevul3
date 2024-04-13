void RenderView::didSelectAutoFillSuggestion(const WebKit::WebNode& node,
                                             const WebKit::WebString& value,
                                             const WebKit::WebString& label,
                                             int unique_id) {
  autofill_helper_.DidSelectAutoFillSuggestion(node, value, label, unique_id);
}
