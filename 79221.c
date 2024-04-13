DevToolsConfirmInfoBarDelegate::~DevToolsConfirmInfoBarDelegate() {
  if (!callback_.is_null())
    callback_.Run(false);
}
