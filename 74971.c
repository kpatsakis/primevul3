void RenderView::willSendSubmitEvent(WebKit::WebFrame* frame,
    const WebKit::WebFormElement& form) {
  NavigationState* navigation_state =
      NavigationState::FromDataSource(frame->dataSource());
  navigation_state->set_password_form_data(
      PasswordFormDomManager::CreatePasswordForm(form));
}
