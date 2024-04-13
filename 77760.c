ConfirmEmailDialogDelegate::ConfirmEmailDialogDelegate(
    content::WebContents* contents,
    const std::string& last_email,
    const std::string& email,
    Callback callback)
  : TabModalConfirmDialogDelegate(contents),
    last_email_(last_email),
    email_(email),
    callback_(callback),
    web_contents_(contents) {
}
