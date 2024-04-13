void ConfirmEmailDialogDelegate::OnLinkClicked(
    WindowOpenDisposition disposition) {
  content::OpenURLParams params(
      GURL(chrome::kChromeSyncMergeTroubleshootingURL),
      content::Referrer(),
      NEW_POPUP,
      content::PAGE_TRANSITION_AUTO_TOPLEVEL,
      false);
  web_contents_->OpenURL(params);
}
