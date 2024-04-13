void OneClickSigninHelper::PasswordSubmitted(
    const autofill::PasswordForm& form) {
  if (gaia::IsGaiaSignonRealm(GURL(form.signon_realm))) {
    VLOG(1) << "OneClickSigninHelper::DidNavigateAnyFrame: got password";
    password_ = UTF16ToUTF8(form.password_value);
  }
}
