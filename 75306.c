void PrintWebViewHelper::PrintPreviewContext::InitWithNode(
    const WebKit::WebNode& web_node) {
  DCHECK(!web_node.isNull());
  DCHECK(!IsRendering());
  state_ = INITIALIZED;
  frame_ = web_node.document().frame();
  node_ = web_node;
}
