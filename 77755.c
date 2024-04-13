bool OneClickSigninHelper::CanOffer(content::WebContents* web_contents,
                                    CanOfferFor can_offer_for,
                                    const std::string& email,
                                    std::string* error_message) {
  DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));
    VLOG(1) << "OneClickSigninHelper::CanOffer";

  if (error_message)
    error_message->clear();

  if (!web_contents)
    return false;

  if (web_contents->GetBrowserContext()->IsOffTheRecord())
    return false;

  Profile* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
  if (!profile)
    return false;

  SigninManager* manager =
      SigninManagerFactory::GetForProfile(profile);
  if (manager && !manager->IsSigninAllowed())
    return false;

  if (can_offer_for == CAN_OFFER_FOR_INTERSTITAL_ONLY &&
      !profile->GetPrefs()->GetBoolean(prefs::kReverseAutologinEnabled))
    return false;

  if (!ChromeSigninManagerDelegate::ProfileAllowsSigninCookies(profile))
    return false;

  if (!email.empty()) {
    if (!manager)
      return false;

    std::string current_email = manager->GetAuthenticatedUsername();
    const bool same_email = gaia::AreEmailsSame(current_email, email);
    if (!current_email.empty() && !same_email) {
      UMA_HISTOGRAM_ENUMERATION("Signin.Reauth",
                                signin::HISTOGRAM_ACCOUNT_MISSMATCH,
                                signin::HISTOGRAM_MAX);
      if (error_message) {
        error_message->assign(
            l10n_util::GetStringFUTF8(IDS_SYNC_WRONG_EMAIL,
                                      UTF8ToUTF16(current_email)));
      }
      return false;
    }

    if (!manager->IsAllowedUsername(email)) {
      if (error_message) {
        error_message->assign(
            l10n_util::GetStringUTF8(IDS_SYNC_LOGIN_NAME_PROHIBITED));
      }
      return false;
    }

    if (g_browser_process && !same_email) {
      ProfileManager* manager = g_browser_process->profile_manager();
      if (manager) {
        string16 email16 = UTF8ToUTF16(email);
        ProfileInfoCache& cache = manager->GetProfileInfoCache();

        for (size_t i = 0; i < cache.GetNumberOfProfiles(); ++i) {
          if (email16 == cache.GetUserNameOfProfileAtIndex(i)) {
            if (error_message) {
              error_message->assign(
                  l10n_util::GetStringUTF8(IDS_SYNC_USER_NAME_IN_USE_ERROR));
            }
            return false;
          }
        }
      }
    }

    if (can_offer_for == CAN_OFFER_FOR_INTERSTITAL_ONLY) {
      const ListValue* rejected_emails = profile->GetPrefs()->GetList(
          prefs::kReverseAutologinRejectedEmailList);
      if (!rejected_emails->empty()) {
        base::ListValue::const_iterator iter = rejected_emails->Find(
            base::StringValue(email));
        if (iter != rejected_emails->end())
          return false;
      }
    }
  }

  VLOG(1) << "OneClickSigninHelper::CanOffer: yes we can";
  return true;
}
