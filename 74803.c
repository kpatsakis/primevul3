void RenderView::OnGetAccessibilityTree() {
  if (accessibility_.get())
    accessibility_->clear();
    accessibility_.reset(WebAccessibilityCache::create());
    accessibility_->initialize(webview());

  WebAccessibilityObject src_tree = webview()->accessibilityObject();
  webkit_glue::WebAccessibility dst_tree(src_tree, accessibility_.get());
  Send(new ViewHostMsg_AccessibilityTree(routing_id_, dst_tree));
}
