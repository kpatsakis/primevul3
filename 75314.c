void PrintWebViewHelper::OnPrintNodeUnderContextMenu() {
  const WebNode& context_menu_node = render_view()->GetContextMenuNode();
  if (context_menu_node.isNull()) {
    NOTREACHED();
    return;
  }

  if (is_preview_enabled_) {
    print_preview_context_.InitWithNode(context_menu_node);
    RequestPrintPreview();
  } else {
    WebNode duplicate_node(context_menu_node);
    Print(duplicate_node.document().frame(), duplicate_node);
  }
}
