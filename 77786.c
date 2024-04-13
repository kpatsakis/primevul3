void OneClickSigninHelper::ShowSigninErrorBubble(Browser* browser,
                                                 const std::string& error) {
  DCHECK(!error.empty());

  browser->window()->ShowOneClickSigninBubble(
      BrowserWindow::ONE_CLICK_SIGNIN_BUBBLE_TYPE_BUBBLE,
      string16(), /* no SAML email */
      UTF8ToUTF16(error),
      BrowserWindow::StartSyncCallback());
}
