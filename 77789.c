StartSyncArgs::StartSyncArgs(Profile* profile,
                             Browser* browser,
                             OneClickSigninHelper::AutoAccept auto_accept,
                             const std::string& session_index,
                             const std::string& email,
                             const std::string& password,
                             content::WebContents* web_contents,
                             bool untrusted_confirmation_required,
                             signin::Source source,
                             OneClickSigninSyncStarter::Callback callback)
    : profile(profile),
      browser(browser),
      auto_accept(auto_accept),
      session_index(session_index),
      email(email),
      password(password),
      web_contents(web_contents),
      source(source),
      callback(callback) {
  if (untrusted_confirmation_required) {
    confirmation_required = OneClickSigninSyncStarter::CONFIRM_UNTRUSTED_SIGNIN;
  } else if (source == signin::SOURCE_SETTINGS ||
             source == signin::SOURCE_WEBSTORE_INSTALL) {
    confirmation_required = OneClickSigninSyncStarter::NO_CONFIRMATION;
  } else {
    confirmation_required = OneClickSigninSyncStarter::CONFIRM_AFTER_SIGNIN;
  }
}
