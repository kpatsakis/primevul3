static void list_response(const char *extname, const mbentry_t *mbentry,
                          uint32_t attributes, struct listargs *listargs)
{
    int r;
    struct statusdata sdata = STATUSDATA_INIT;
    struct buf specialuse = BUF_INITIALIZER;

    if ((attributes & MBOX_ATTRIBUTE_NONEXISTENT)) {
        if (!(listargs->cmd == LIST_CMD_EXTENDED)) {
            attributes |= MBOX_ATTRIBUTE_NOSELECT;
            attributes &= ~MBOX_ATTRIBUTE_NONEXISTENT;
        }
    }

    else if (listargs->scan) {
        /* SCAN mailbox for content */

        if (!strcmpsafe(mbentry->name, index_mboxname(imapd_index))) {
            /* currently selected mailbox */
            if (!index_scan(imapd_index, listargs->scan))
                return; /* no matching messages */
        }
        else {
            /* other local mailbox */
            struct index_state *state;
            struct index_init init;
            int doclose = 0;

            memset(&init, 0, sizeof(struct index_init));
            init.userid = imapd_userid;
            init.authstate = imapd_authstate;
            init.out = imapd_out;

            r = index_open(mbentry->name, &init, &state);

            if (!r)
                doclose = 1;

            if (!r && index_hasrights(state, ACL_READ)) {
                r = (imapd_userisadmin || index_hasrights(state, ACL_LOOKUP)) ?
                    IMAP_PERMISSION_DENIED : IMAP_MAILBOX_NONEXISTENT;
            }

            if (!r) {
                if (!index_scan(state, listargs->scan)) {
                    r = -1;  /* no matching messages */
                }
            }

            if (doclose) index_close(&state);

            if (r) return;
        }
    }

    /* figure out \Has(No)Children if necessary
       This is mainly used for LIST (SUBSCRIBED) RETURN (CHILDREN)
    */
    uint32_t have_childinfo =
        MBOX_ATTRIBUTE_HASCHILDREN | MBOX_ATTRIBUTE_HASNOCHILDREN;
    if ((listargs->ret & LIST_RET_CHILDREN) && !(attributes & have_childinfo)) {
        if (imapd_namespace.isalt && !strcmp(extname, "INBOX")) {
            /* don't look inside INBOX under altnamespace, its children aren't children */
        }
        else {
            char *intname = NULL, *freeme = NULL;

            /* if we got here via subscribed_cb, mbentry isn't set */
            if (mbentry)
                intname = mbentry->name;
            else
                intname = freeme = mboxname_from_external(extname, &imapd_namespace, imapd_userid);

            mboxlist_mboxtree(intname, set_haschildren, &attributes, MBOXTREE_SKIP_ROOT);
            if (freeme) free(freeme);
        }

        if (!(attributes & MBOX_ATTRIBUTE_HASCHILDREN))
            attributes |= MBOX_ATTRIBUTE_HASNOCHILDREN;
    }

    if (attributes & (MBOX_ATTRIBUTE_NONEXISTENT | MBOX_ATTRIBUTE_NOSELECT)) {
        int keep = 0;
        /* extended get told everything */
        if (listargs->cmd == LIST_CMD_EXTENDED) {
            keep = 1;
        }
        /* we have to mention this, it has children */
        if (listargs->cmd == LIST_CMD_LSUB) {
            /* subscribed children need a mention */
            if (attributes & MBOX_ATTRIBUTE_CHILDINFO_SUBSCRIBED)
                keep = 1;
            /* if mupdate is configured we can't drop out, we might
             * be a backend and need to report folders that don't
             * exist on this backend - this is awful and complex
             * and brittle and should be changed */
            if (config_mupdate_server)
                keep = 1;
        }
        else if (attributes & MBOX_ATTRIBUTE_HASCHILDREN)
            keep = 1;

        if (!keep) return;
    }

    if (listargs->cmd == LIST_CMD_LSUB) {
        /* \Noselect has a special second meaning with (R)LSUB */
        if ( !(attributes & MBOX_ATTRIBUTE_SUBSCRIBED)
             && attributes & MBOX_ATTRIBUTE_CHILDINFO_SUBSCRIBED)
            attributes |= MBOX_ATTRIBUTE_NOSELECT | MBOX_ATTRIBUTE_HASCHILDREN;
        attributes &= ~MBOX_ATTRIBUTE_SUBSCRIBED;
    }

    /* As CHILDINFO extended data item is not allowed if the
     * RECURSIVEMATCH selection option is not specified */
    if (!(listargs->sel & LIST_SEL_RECURSIVEMATCH)) {
        attributes &= ~MBOX_ATTRIBUTE_CHILDINFO_SUBSCRIBED;
    }

    /* no inferiors means no children (this basically means the INBOX
     * in alt namespace mode */
    if (attributes & MBOX_ATTRIBUTE_NOINFERIORS)
        attributes &= ~MBOX_ATTRIBUTE_HASCHILDREN;

    /* you can't have both!  If it's had children, it has children */
    if (attributes & MBOX_ATTRIBUTE_HASCHILDREN)
        attributes &= ~MBOX_ATTRIBUTE_HASNOCHILDREN;

    /* remove redundant flags */
    if (listargs->cmd == LIST_CMD_EXTENDED) {
        /* \NoInferiors implies \HasNoChildren */
        if (attributes & MBOX_ATTRIBUTE_NOINFERIORS)
            attributes &= ~MBOX_ATTRIBUTE_HASNOCHILDREN;
        /* \NonExistent implies \Noselect */
        if (attributes & MBOX_ATTRIBUTE_NONEXISTENT)
            attributes &= ~MBOX_ATTRIBUTE_NOSELECT;
    }

    if (config_getswitch(IMAPOPT_SPECIALUSEALWAYS) ||
        listargs->cmd == LIST_CMD_XLIST ||
        listargs->ret & LIST_RET_SPECIALUSE) {
        specialuse_flags(mbentry, &specialuse, listargs->cmd == LIST_CMD_XLIST);
    }

    if (listargs->sel & LIST_SEL_SPECIALUSE) {
        /* check that this IS a specialuse folder */
        if (!buf_len(&specialuse)) return;
    }

    /* can we read the status data ? */
    if ((listargs->ret & LIST_RET_STATUS) && mbentry) {
        r = imapd_statusdata(mbentry->name, listargs->statusitems, &sdata);
        if (r) {
            /* RFC 5819: the STATUS response MUST NOT be returned and the
             * LIST response MUST include the \NoSelect attribute. */
            attributes |= MBOX_ATTRIBUTE_NOSELECT;
        }
    }

    print_listresponse(listargs->cmd, extname,
                       imapd_namespace.hier_sep, attributes, &specialuse);
    buf_free(&specialuse);

    if ((listargs->ret & LIST_RET_STATUS) &&
        !(attributes & MBOX_ATTRIBUTE_NOSELECT)) {
        /* output the status line now, per rfc 5819 */
        if (mbentry) print_statusline(extname, listargs->statusitems, &sdata);
    }

    if ((listargs->ret & LIST_RET_MYRIGHTS) &&
        !(attributes & MBOX_ATTRIBUTE_NOSELECT)) {
        if (mbentry) printmyrights(extname, mbentry);
    }

    if ((listargs->ret & LIST_RET_METADATA) &&
        !(attributes & MBOX_ATTRIBUTE_NOSELECT)) {
        if (mbentry)
            printmetadata(mbentry, &listargs->metaitems, &listargs->metaopts);
    }
}
