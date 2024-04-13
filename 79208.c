void DevToolsWindow::ShowDevToolsConfirmInfoBar(
    const string16& message,
    const InfoBarCallback& callback) {
  DevToolsConfirmInfoBarDelegate::Create(
      IsDocked() ?
          InfoBarService::FromWebContents(GetInspectedWebContents()) :
          InfoBarService::FromWebContents(web_contents_),
      callback, message);
}
