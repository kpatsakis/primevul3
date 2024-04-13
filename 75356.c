string16 BlockedPluginInfoBarDelegate::GetMessageText() const {
  return l10n_util::GetStringFUTF16(IDS_PLUGIN_NOT_AUTHORIZED, name_);
}
