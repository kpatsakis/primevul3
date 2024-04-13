void RenderView::didClearAutoFillSelection(const WebKit::WebNode& node) {
  autofill_helper_.DidClearAutoFillSelection(node);
}
