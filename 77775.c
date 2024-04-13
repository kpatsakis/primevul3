void ConfirmEmailDialogDelegate::OnClosed() {
  base::ResetAndReturn(&callback_).Run(CLOSE);
}
