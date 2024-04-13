void StartSync(const StartSyncArgs& args,
               OneClickSigninSyncStarter::StartSyncMode start_mode) {
  if (start_mode == OneClickSigninSyncStarter::UNDO_SYNC) {
    LogOneClickHistogramValue(one_click_signin::HISTOGRAM_UNDO);
    return;
  }

  new OneClickSigninSyncStarter(args.profile, args.browser, args.session_index,
                                args.email, args.password,
                                "" /* oauth_code */, start_mode,
                                args.web_contents,
                                args.confirmation_required,
                                args.callback);

  int action = one_click_signin::HISTOGRAM_MAX;
  switch (args.auto_accept) {
    case OneClickSigninHelper::AUTO_ACCEPT_EXPLICIT:
      break;
    case OneClickSigninHelper::AUTO_ACCEPT_ACCEPTED:
      action =
          start_mode == OneClickSigninSyncStarter::SYNC_WITH_DEFAULT_SETTINGS ?
              one_click_signin::HISTOGRAM_AUTO_WITH_DEFAULTS :
              one_click_signin::HISTOGRAM_AUTO_WITH_ADVANCED;
      break;
    case OneClickSigninHelper::AUTO_ACCEPT_CONFIGURE:
      DCHECK(start_mode == OneClickSigninSyncStarter::CONFIGURE_SYNC_FIRST);
      action = one_click_signin::HISTOGRAM_AUTO_WITH_ADVANCED;
      break;
    default:
      NOTREACHED() << "Invalid auto_accept: " << args.auto_accept;
      break;
  }
  if (action != one_click_signin::HISTOGRAM_MAX)
    LogOneClickHistogramValue(action);
}
