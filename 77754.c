void ConfirmEmailDialogDelegate::AskForConfirmation(
    content::WebContents* contents,
    const std::string& last_email,
    const std::string& email,
    Callback callback) {
  TabModalConfirmDialog::Create(
      new ConfirmEmailDialogDelegate(contents, last_email, email,
                                     callback), contents);
}
