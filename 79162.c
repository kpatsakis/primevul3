string16 DevToolsConfirmInfoBarDelegate::GetButtonLabel(
    InfoBarButton button) const {
  return l10n_util::GetStringUTF16((button == BUTTON_OK) ?
      IDS_DEV_TOOLS_CONFIRM_ALLOW_BUTTON : IDS_DEV_TOOLS_CONFIRM_DENY_BUTTON);
}
