DevToolsConfirmInfoBarDelegate::DevToolsConfirmInfoBarDelegate(
    InfoBarService* infobar_service,
    const DevToolsWindow::InfoBarCallback& callback,
    const string16& message)
    : ConfirmInfoBarDelegate(infobar_service),
      callback_(callback),
      message_(message) {
}
