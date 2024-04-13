void AutoFillHelper::DidSelectAutoFillSuggestion(const WebNode& node,
                                                 const WebString& value,
                                                 const WebString& label,
                                                 int unique_id) {
  DidClearAutoFillSelection(node);
  QueryAutoFillFormData(node, value, label, unique_id, AUTOFILL_PREVIEW);
}
