void DevToolsConfirmInfoBarDelegate::Create(
    InfoBarService* infobar_service,
    const DevToolsWindow::InfoBarCallback& callback,
    const string16& message) {
  if (!infobar_service) {
    callback.Run(false);
    return;
  }

  infobar_service->AddInfoBar(scoped_ptr<InfoBarDelegate>(
      new DevToolsConfirmInfoBarDelegate(infobar_service, callback, message)));
}
