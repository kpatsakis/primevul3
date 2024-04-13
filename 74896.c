void RenderView::didAcceptAutocompleteSuggestion(
    const WebKit::WebInputElement& user_element) {
#if defined(WEBKIT_BUG_41283_IS_FIXED)
  bool result = password_autocomplete_manager_.FillPassword(user_element);
  DCHECK(result);
#endif
}
