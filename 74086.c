int service_init(int argc, char **argv, char **envp)
{
    int opt, events;

    if (geteuid() == 0) fatal("must run as the Cyrus user", EC_USAGE);
    setproctitle_init(argc, argv, envp);

    /* set signal handlers */
    signals_set_shutdown(&shut_down);
    signal(SIGPIPE, SIG_IGN);

    /* load the SASL plugins */
    global_sasl_init(1, 1, mysasl_cb);

    /* open the mboxlist, we'll need it for real work */
    mboxlist_init(0);
    mboxlist_open(NULL);

    /* open the quota db, we'll need it for real work */
    quotadb_init(0);
    quotadb_open(NULL);

    /* open the user deny db */
    denydb_init(0);
    denydb_open(0);

    /* setup for sending IMAP IDLE notifications */
    idle_init();

    /* setup for mailbox event notifications */
    events = mboxevent_init();
    apns_enabled =
      (events & EVENT_APPLEPUSHSERVICE) && config_getstring(IMAPOPT_APS_TOPIC);

    search_attr_init();

    /* create connection to the SNMP listener, if available. */
    snmp_connect(); /* ignore return code */
    snmp_set_str(SERVER_NAME_VERSION,cyrus_version());

    while ((opt = getopt(argc, argv, "Np:sq")) != EOF) {
        switch (opt) {
        case 's': /* imaps (do starttls right away) */
            imaps = 1;
            if (!tls_enabled()) {
                syslog(LOG_ERR, "imaps: required OpenSSL options not present");
                fatal("imaps: required OpenSSL options not present",
                      EC_CONFIG);
            }
            break;
        case 'p': /* external protection */
            extprops_ssf = atoi(optarg);
            break;
        case 'N': /* bypass SASL password check.  Not recommended unless
                   * you know what you're doing! */
            nosaslpasswdcheck = 1;
            break;
        case 'q': /* don't enforce quotas */
            ignorequota = 1;
            break;
        default:
            break;
        }
    }

    /* Initialize the annotatemore extention */
    if (config_mupdate_server)
        annotate_init(annotate_fetch_proxy, annotate_store_proxy);
    else
        annotate_init(NULL, NULL);
    annotatemore_open();

    if (config_getswitch(IMAPOPT_STATUSCACHE)) {
        statuscache_open();
    }

    /* Create a protgroup for input from the client and selected backend */
    protin = protgroup_new(2);

    return 0;
}
