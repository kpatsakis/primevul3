OneClickSigninHelper::OneClickSigninHelper(content::WebContents* web_contents,
                                           PasswordManager* password_manager)
    : content::WebContentsObserver(web_contents),
      showing_signin_(false),
      auto_accept_(AUTO_ACCEPT_NONE),
      source_(signin::SOURCE_UNKNOWN),
      switched_to_advanced_(false),
      untrusted_navigations_since_signin_visit_(0),
      untrusted_confirmation_required_(false),
      do_not_clear_pending_email_(false),
      do_not_start_sync_for_testing_(false),
      weak_pointer_factory_(this) {
  if (password_manager) {
    password_manager->AddSubmissionCallback(
        base::Bind(&OneClickSigninHelper::PasswordSubmitted,
                   weak_pointer_factory_.GetWeakPtr()));
  }
}
