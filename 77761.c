void OneClickSigninHelper::CreateForWebContentsWithPasswordManager(
    content::WebContents* contents,
    PasswordManager* password_manager) {
  if (!FromWebContents(contents)) {
    contents->SetUserData(UserDataKey(),
                          new OneClickSigninHelper(contents, password_manager));
  }
}
