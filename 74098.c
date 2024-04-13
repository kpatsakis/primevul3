static int xfer_backport_seen_item(struct xfer_item *item,
                                   struct seen *seendb)
{
    struct mailbox *mailbox = item->mailbox;
    struct seqset *outlist = NULL;
    struct seendata sd = SEENDATA_INITIALIZER;
    int r;

    outlist = seqset_init(mailbox->i.last_uid, SEQ_MERGE);

    struct mailbox_iter *iter = mailbox_iter_init(mailbox, 0, ITER_SKIP_EXPUNGED);

    const message_t *msg;
    while ((msg = mailbox_iter_step(iter))) {
        const struct index_record *record = msg_record(msg);
        if (record->system_flags & FLAG_SEEN)
            seqset_add(outlist, record->uid, 1);
        else
            seqset_add(outlist, record->uid, 0);
    }

    mailbox_iter_done(&iter);

    sd.lastread = mailbox->i.recenttime;
    sd.lastuid = mailbox->i.recentuid;
    sd.lastchange = mailbox->i.last_appenddate;
    sd.seenuids = seqset_cstring(outlist);
    if (!sd.seenuids) sd.seenuids = xstrdup("");

    r = seen_write(seendb, mailbox->uniqueid, &sd);

    seen_freedata(&sd);
    seqset_free(outlist);

    return r;
}
