static void cmd_idle(char *tag)
{
    int c = EOF;
    int flags;
    static struct buf arg;
    static int idle_period = -1;
    static time_t idle_timeout = -1;
    struct timespec deadline = { 0, 0 };

    if (idle_timeout == -1) {
        idle_timeout = config_getint(IMAPOPT_IMAPIDLETIMEOUT);
        if (idle_timeout <= 0) {
            idle_timeout = config_getint(IMAPOPT_TIMEOUT);
        }
        idle_timeout *= 60; /* unit is minutes */
    }

    if (idle_timeout > 0) {
        errno = 0;
        if (clock_gettime(CLOCK_MONOTONIC, &deadline) == -1) {
            syslog(LOG_ERR, "clock_gettime (%d %m): error reading clock",
                   errno);
        } else {
            deadline.tv_sec += idle_timeout;
        }
    }

    if (!backend_current) {  /* Local mailbox */

        /* Tell client we are idling and waiting for end of command */
        prot_printf(imapd_out, "+ idling\r\n");
        prot_flush(imapd_out);

        /* Start doing mailbox updates */
        index_check(imapd_index, 1, 0);
        idle_start(index_mboxname(imapd_index));
        /* use this flag so if getc causes a shutdown due to
         * connection abort we tell idled about it */
        idling = 1;

        index_release(imapd_index);
        while ((flags = idle_wait(imapd_in->fd))) {
            if (deadline_exceeded(&deadline)) {
                syslog(LOG_DEBUG, "timeout for user '%s' while idling",
                       imapd_userid);
                shut_down(0);
                break;
            }

            if (flags & IDLE_INPUT) {
                /* Get continuation data */
                c = getword(imapd_in, &arg);
                break;
            }

            /* Send unsolicited untagged responses to the client */
            if (flags & IDLE_MAILBOX)
                index_check(imapd_index, 1, 0);

            if (flags & IDLE_ALERT) {
                char shut[MAX_MAILBOX_PATH+1];
                if (! imapd_userisadmin &&
                    (shutdown_file(shut, sizeof(shut)) ||
                     (imapd_userid &&
                      userdeny(imapd_userid, config_ident, shut, sizeof(shut))))) {
                    char *p;
                    for (p = shut; *p == '['; p++); /* can't have [ be first char */
                    prot_printf(imapd_out, "* BYE [ALERT] %s\r\n", p);
                    shut_down(0);
                }
            }

            index_release(imapd_index);
            prot_flush(imapd_out);
        }

        /* Stop updates and do any necessary cleanup */
        idling = 0;
        idle_stop(index_mboxname(imapd_index));
    }
    else {  /* Remote mailbox */
        int done = 0;
        enum { shutdown_skip, shutdown_bye, shutdown_silent } shutdown = shutdown_skip;
        char buf[2048];

        /* get polling period */
        if (idle_period == -1) {
            idle_period = config_getint(IMAPOPT_IMAPIDLEPOLL);
        }

        if (CAPA(backend_current, CAPA_IDLE)) {
            /* Start IDLE on backend */
            prot_printf(backend_current->out, "%s IDLE\r\n", tag);
            if (!prot_fgets(buf, sizeof(buf), backend_current->in)) {

                /* If we received nothing from the backend, fail */
                prot_printf(imapd_out, "%s NO %s\r\n", tag,
                            error_message(IMAP_SERVER_UNAVAILABLE));
                return;
            }
            if (buf[0] != '+') {
                /* If we received anything but a continuation response,
                   spit out what we received and quit */
                prot_write(imapd_out, buf, strlen(buf));
                return;
            }
        }

        /* Tell client we are idling and waiting for end of command */
        prot_printf(imapd_out, "+ idling\r\n");
        prot_flush(imapd_out);

        /* Pipe updates to client while waiting for end of command */
        while (!done) {
            if (deadline_exceeded(&deadline)) {
                syslog(LOG_DEBUG,
                       "timeout for user '%s' while idling on remote mailbox",
                       imapd_userid);
                shutdown = shutdown_silent;
                goto done;
            }

            /* Flush any buffered output */
            prot_flush(imapd_out);

            /* Check for shutdown file */
            if (!imapd_userisadmin &&
                (shutdown_file(buf, sizeof(buf)) ||
                 (imapd_userid &&
                  userdeny(imapd_userid, config_ident, buf, sizeof(buf))))) {
                done = 1;
                shutdown = shutdown_bye;
                goto done;
            }

            done = proxy_check_input(protin, imapd_in, imapd_out,
                                     backend_current->in, NULL, idle_period);

            /* If not running IDLE on backend, poll the mailbox for updates */
            if (!CAPA(backend_current, CAPA_IDLE)) {
                imapd_check(NULL, 0);
            }
        }

        /* Get continuation data */
        c = getword(imapd_in, &arg);

      done:
        if (CAPA(backend_current, CAPA_IDLE)) {
            /* Either the client timed out, or ended the command.
               In either case we're done, so terminate IDLE on backend */
            prot_printf(backend_current->out, "Done\r\n");
            pipe_until_tag(backend_current, tag, 0);
        }

        switch (shutdown) {
        case shutdown_bye:
            ;
            char *p;

            for (p = buf; *p == '['; p++); /* can't have [ be first char */
            prot_printf(imapd_out, "* BYE [ALERT] %s\r\n", p);
            /* fallthrough */
        case shutdown_silent:
            shut_down(0);
            break;
        case shutdown_skip:
        default:
            break;
        }
    }

    imapd_check(NULL, 1);

    if (c != EOF) {
        if (!strcasecmp(arg.s, "Done") &&
            (c = (c == '\r') ? prot_getc(imapd_in) : c) == '\n') {
            prot_printf(imapd_out, "%s OK %s\r\n", tag,
                        error_message(IMAP_OK_COMPLETED));
        }
        else {
            prot_printf(imapd_out,
                        "%s BAD Invalid Idle continuation\r\n", tag);
            eatline(imapd_in, c);
        }
    }
}
