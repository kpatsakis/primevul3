void AutoFillHelper::DidClearAutoFillSelection(const WebNode& node) {
  form_manager_.ClearPreviewedFormWithNode(node);
}
