static void cmd_xapplepushservice(const char *tag,
                                  struct applepushserviceargs *applepushserviceargs)
{
    int r = 0;
    strarray_t notif_mailboxes = STRARRAY_INITIALIZER;
    int i;
    mbentry_t *mbentry = NULL;

    const char *aps_topic = config_getstring(IMAPOPT_APS_TOPIC);
    if (!aps_topic) {
        syslog(LOG_ERR,
               "aps_topic not configured, can't complete XAPPLEPUSHSERVICE response");
        prot_printf(imapd_out, "%s NO Server configuration error\r\n", tag);
        return;
    }

    if (!buf_len(&applepushserviceargs->aps_account_id)) {
        prot_printf(imapd_out, "%s NO Missing APNS account ID\r\n", tag);
        return;
    }
    if (!buf_len(&applepushserviceargs->aps_device_token)) {
        prot_printf(imapd_out, "%s NO Missing APNS device token\r\n", tag);
        return;
    }
    if (!buf_len(&applepushserviceargs->aps_subtopic)) {
        prot_printf(imapd_out, "%s NO Missing APNS sub-topic\r\n", tag);
        return;
    }

    if (applepushserviceargs->aps_version == 1) {
        strarray_truncate(&applepushserviceargs->mailboxes, 0);
        strarray_push(&applepushserviceargs->mailboxes, "INBOX");
        applepushserviceargs->aps_version = 1;
    }
    else {
        applepushserviceargs->aps_version = 2;
    }

    for (i = 0; i < strarray_size(&applepushserviceargs->mailboxes); i++) {
        const char *name = strarray_nth(&applepushserviceargs->mailboxes, i);
        char *intname =
            mboxname_from_external(name, &imapd_namespace, imapd_userid);
        r = mlookup(tag, name, intname, &mbentry);
        if (!r && mbentry->mbtype == 0) {
            strarray_push(&notif_mailboxes, name);
            if (applepushserviceargs->aps_version >= 2) {
                prot_puts(imapd_out, "* XAPPLEPUSHSERVICE \"mailbox\" ");
                prot_printstring(imapd_out, name);
                prot_puts(imapd_out, "\r\n");
            }
        }
        mboxlist_entry_free(&mbentry);
        free(intname);
    }

    prot_printf(imapd_out,
                "* XAPPLEPUSHSERVICE \"aps-version\" \"%d\" \"aps-topic\" \"%s\"\r\n",
                applepushserviceargs->aps_version, aps_topic);
    prot_printf(imapd_out, "%s OK XAPPLEPUSHSERVICE completed.\r\n", tag);

    struct mboxevent *mboxevent = mboxevent_new(EVENT_APPLEPUSHSERVICE);
    mboxevent_set_applepushservice(mboxevent, applepushserviceargs,
                                   &notif_mailboxes, imapd_userid);
    mboxevent_notify(&mboxevent);
    mboxevent_free(&mboxevent);

    buf_release(&applepushserviceargs->aps_account_id);
    buf_release(&applepushserviceargs->aps_device_token);
    buf_release(&applepushserviceargs->aps_subtopic);
    strarray_fini(&applepushserviceargs->mailboxes);
    strarray_fini(&notif_mailboxes);
}
