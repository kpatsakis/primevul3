void RenderView::textFieldDidBeginEditing(
    const WebKit::WebInputElement& element) {
#if defined(WEBKIT_BUG_41283_IS_FIXED)
  password_autocomplete_manager_.TextFieldDidBeginEditing(element);
#endif
}
