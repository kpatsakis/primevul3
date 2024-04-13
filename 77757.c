OneClickSigninHelper::Offer OneClickSigninHelper::CanOfferOnIOThreadImpl(
    const GURL& url,
    const std::string& referrer,
    base::SupportsUserData* request,
    ProfileIOData* io_data) {
  if (!gaia::IsGaiaSignonRealm(url.GetOrigin()))
    return IGNORE_REQUEST;

  if (!io_data)
    return DONT_OFFER;

  if (io_data->is_incognito())
    return DONT_OFFER;

  if (!SigninManager::IsSigninAllowedOnIOThread(io_data))
    return DONT_OFFER;

  if (!io_data->reverse_autologin_enabled()->GetValue())
    return DONT_OFFER;

  if (!io_data->google_services_username()->GetValue().empty())
    return DONT_OFFER;

  if (!ChromeSigninManagerDelegate::SettingsAllowSigninCookies(
          io_data->GetCookieSettings()))
    return DONT_OFFER;

  const std::string& pending_email = io_data->reverse_autologin_pending_email();
  if (!pending_email.empty()) {
    if (!SigninManager::IsUsernameAllowedByPolicy(pending_email,
            io_data->google_services_username_pattern()->GetValue())) {
      return DONT_OFFER;
    }

    std::vector<std::string> rejected_emails =
        io_data->one_click_signin_rejected_email_list()->GetValue();
    if (std::count_if(rejected_emails.begin(), rejected_emails.end(),
                      std::bind2nd(std::equal_to<std::string>(),
                                   pending_email)) > 0) {
      return DONT_OFFER;
    }

    if (io_data->signin_names()->GetEmails().count(
            UTF8ToUTF16(pending_email)) > 0) {
      return DONT_OFFER;
    }
  }

  return CAN_OFFER;
}
