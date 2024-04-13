void OneClickSigninHelper::CleanTransientState() {
  VLOG(1) << "OneClickSigninHelper::CleanTransientState";
  showing_signin_ = false;
  email_.clear();
  password_.clear();
  auto_accept_ = AUTO_ACCEPT_NONE;
  source_ = signin::SOURCE_UNKNOWN;
  switched_to_advanced_ = false;
  continue_url_ = GURL();
  untrusted_navigations_since_signin_visit_ = 0;
  untrusted_confirmation_required_ = false;
  error_message_.clear();

  if (!do_not_clear_pending_email_) {
    Profile* profile =
        Profile::FromBrowserContext(web_contents()->GetBrowserContext());
    content::BrowserThread::PostTask(
        content::BrowserThread::IO, FROM_HERE,
        base::Bind(&ClearPendingEmailOnIOThread,
                   base::Unretained(profile->GetResourceContext())));
  }
}
