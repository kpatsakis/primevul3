string16 ConfirmEmailDialogDelegate::GetMessage() {
  return l10n_util::GetStringFUTF16(
      IDS_ONE_CLICK_SIGNIN_CONFIRM_EMAIL_DIALOG_MESSAGE,
      UTF8ToUTF16(last_email_), UTF8ToUTF16(email_));
}
