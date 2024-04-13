static void cmd_reconstruct(const char *tag, const char *name, int recursive)
{
    int r = 0;
    char quotaroot[MAX_MAILBOX_BUFFER];
    mbentry_t *mbentry = NULL;
    struct mailbox *mailbox = NULL;

    /* administrators only please */
    if (!imapd_userisadmin)
        r = IMAP_PERMISSION_DENIED;

    char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);

    if (!r && !strcmpsafe(intname, index_mboxname(imapd_index)))
        r = IMAP_MAILBOX_LOCKED;

    if (!r) {
        r = mlookup(tag, name, intname, &mbentry);
    }
    if (r == IMAP_MAILBOX_MOVED) {
        free(intname);
        return;
    }

    if (!r && (mbentry->mbtype & MBTYPE_REMOTE)) {
        /* remote mailbox */
        imapd_refer(tag, mbentry->server, name);
        mboxlist_entry_free(&mbentry);
        free(intname);
        return;
    }

    mboxlist_entry_free(&mbentry);

    /* local mailbox */
    if (!r) {
        int pid;

        /* Reconstruct it */

        pid = fork();
        if (pid == -1) {
            r = IMAP_SYS_ERROR;
        } else if (pid == 0) {
            char buf[4096];
            int ret;

            /* Child - exec reconstruct*/
            syslog(LOG_NOTICE, "Reconstructing '%s' (%s) for user '%s'",
                   intname, recursive ? "recursive" : "not recursive",
                   imapd_userid);

            fclose(stdin);
            fclose(stdout);
            fclose(stderr);

            ret = snprintf(buf, sizeof(buf), "%s/reconstruct", SBIN_DIR);
            if(ret < 0 || ret >= (int) sizeof(buf)) {
                /* in child, so fatailing won't disconnect our user */
                fatal("reconstruct buffer not sufficiently big", EC_CONFIG);
            }

            if(recursive) {
                execl(buf, buf, "-C", config_filename, "-r", "-f",
                      intname, NULL);
            } else {
                execl(buf, buf, "-C", config_filename, intname, NULL);
            }

            /* if we are here, we have a problem */
            exit(-1);
        } else {
            int status;

            /* Parent, wait on child */
            if(waitpid(pid, &status, 0) < 0) r = IMAP_SYS_ERROR;

            /* Did we fail? */
            if(WEXITSTATUS(status) != 0) r = IMAP_SYS_ERROR;
        }
    }

    /* Still in parent, need to re-quota the mailbox*/

    /* Find its quota root */
    if (!r)
        r = mailbox_open_irl(intname, &mailbox);

    if(!r) {
        if(mailbox->quotaroot) {
            strcpy(quotaroot, mailbox->quotaroot);
        } else {
            strcpy(quotaroot, intname);
        }
        mailbox_close(&mailbox);
    }

    /* Run quota -f */
    if (!r) {
        int pid;

        pid = fork();
        if(pid == -1) {
            r = IMAP_SYS_ERROR;
        } else if(pid == 0) {
            char buf[4096];
            int ret;

            /* Child - exec reconstruct*/
            syslog(LOG_NOTICE,
                   "Regenerating quota roots starting with '%s' for user '%s'",
                   intname, imapd_userid);

            fclose(stdin);
            fclose(stdout);
            fclose(stderr);

            ret = snprintf(buf, sizeof(buf), "%s/quota", SBIN_DIR);
            if(ret < 0 || ret >= (int) sizeof(buf)) {
                /* in child, so fatailing won't disconnect our user */
                fatal("quota buffer not sufficiently big", EC_CONFIG);
            }

            execl(buf, buf, "-C", config_filename, "-f", quotaroot, NULL);

            /* if we are here, we have a problem */
            exit(-1);
        } else {
            int status;

            /* Parent, wait on child */
            if(waitpid(pid, &status, 0) < 0) r = IMAP_SYS_ERROR;

            /* Did we fail? */
            if(WEXITSTATUS(status) != 0) r = IMAP_SYS_ERROR;
        }
    }

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
    } else {
        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
    }

    free(intname);
}
