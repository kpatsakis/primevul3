static void authentication_success(void)
{
    int r;
    struct mboxevent *mboxevent;

    /* authstate already created by mysasl_proxy_policy() */
    imapd_userisadmin = global_authisa(imapd_authstate, IMAPOPT_ADMINS);

    /* Create telemetry log */
    imapd_logfd = telemetry_log(imapd_userid, imapd_in, imapd_out, 0);

    /* Set namespace */
    r = mboxname_init_namespace(&imapd_namespace,
                                imapd_userisadmin || imapd_userisproxyadmin);

    mboxevent_setnamespace(&imapd_namespace);

    if (r) {
        syslog(LOG_ERR, "%s", error_message(r));
        fatal(error_message(r), EC_CONFIG);
    }

    /* Make a copy of the external userid for use in proxying */
    proxy_userid = xstrdup(imapd_userid);

    /* send a Login event notification */
    if ((mboxevent = mboxevent_new(EVENT_LOGIN))) {
        mboxevent_set_access(mboxevent, saslprops.iplocalport,
                             saslprops.ipremoteport, imapd_userid, NULL, 1);

        mboxevent_notify(&mboxevent);
        mboxevent_free(&mboxevent);
    }

#ifdef USE_AUTOCREATE
    autocreate_inbox();
#endif // USE_AUTOCREATE
}
