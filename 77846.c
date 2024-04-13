void RenderFrameImpl::OnContextMenuClosed(
    const CustomContextMenuContext& custom_context) {
  if (custom_context.request_id) {
    ContextMenuClient* client =
        pending_context_menus_.Lookup(custom_context.request_id);
    if (client) {
      client->OnMenuClosed(custom_context.request_id);
      pending_context_menus_.Remove(custom_context.request_id);
    }
  } else {
    context_menu_node_.reset();
  }
}
