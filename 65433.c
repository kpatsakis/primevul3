int imap_copy_messages(struct Context *ctx, struct Header *h, char *dest, int delete)
{
  struct Buffer cmd, sync_cmd;
  char mbox[PATH_MAX];
  char mmbox[PATH_MAX];
  char prompt[PATH_MAX + 64];
  int rc;
  struct ImapMbox mx;
  int err_continue = MUTT_NO;
  int triedcreate = 0;

  struct ImapData *idata = ctx->data;

  if (imap_parse_path(dest, &mx))
  {
    mutt_debug(1, "bad destination %s\n", dest);
    return -1;
  }

  /* check that the save-to folder is in the same account */
  if (mutt_account_match(&(idata->conn->account), &(mx.account)) == 0)
  {
    mutt_debug(3, "%s not same server as %s\n", dest, ctx->path);
    return 1;
  }

  if (h && h->attach_del)
  {
    mutt_debug(3, "#1 Message contains attachments to be deleted\n");
    return 1;
  }

  imap_fix_path(idata, mx.mbox, mbox, sizeof(mbox));
  if (!*mbox)
    mutt_str_strfcpy(mbox, "INBOX", sizeof(mbox));
  imap_munge_mbox_name(idata, mmbox, sizeof(mmbox), mbox);

  /* loop in case of TRYCREATE */
  do
  {
    mutt_buffer_init(&sync_cmd);
    mutt_buffer_init(&cmd);

    /* Null Header* means copy tagged messages */
    if (!h)
    {
      /* if any messages have attachments to delete, fall through to FETCH
       * and APPEND. TODO: Copy what we can with COPY, fall through for the
       * remainder. */
      for (int i = 0; i < ctx->msgcount; i++)
      {
        if (!message_is_tagged(ctx, i))
          continue;

        if (ctx->hdrs[i]->attach_del)
        {
          mutt_debug(3, "#2 Message contains attachments to be deleted\n");
          return 1;
        }

        if (ctx->hdrs[i]->active && ctx->hdrs[i]->changed)
        {
          rc = imap_sync_message_for_copy(idata, ctx->hdrs[i], &sync_cmd, &err_continue);
          if (rc < 0)
          {
            mutt_debug(1, "#1 could not sync\n");
            goto out;
          }
        }
      }

      rc = imap_exec_msgset(idata, "UID COPY", mmbox, MUTT_TAG, 0, 0);
      if (!rc)
      {
        mutt_debug(1, "No messages tagged\n");
        rc = -1;
        goto out;
      }
      else if (rc < 0)
      {
        mutt_debug(1, "#1 could not queue copy\n");
        goto out;
      }
      else
      {
        mutt_message(ngettext("Copying %d message to %s...", "Copying %d messages to %s...", rc),
                     rc, mbox);
      }
    }
    else
    {
      mutt_message(_("Copying message %d to %s..."), h->index + 1, mbox);
      mutt_buffer_printf(&cmd, "UID COPY %u %s", HEADER_DATA(h)->uid, mmbox);

      if (h->active && h->changed)
      {
        rc = imap_sync_message_for_copy(idata, h, &sync_cmd, &err_continue);
        if (rc < 0)
        {
          mutt_debug(1, "#2 could not sync\n");
          goto out;
        }
      }
      rc = imap_exec(idata, cmd.data, IMAP_CMD_QUEUE);
      if (rc < 0)
      {
        mutt_debug(1, "#2 could not queue copy\n");
        goto out;
      }
    }

    /* let's get it on */
    rc = imap_exec(idata, NULL, IMAP_CMD_FAIL_OK);
    if (rc == -2)
    {
      if (triedcreate)
      {
        mutt_debug(1, "Already tried to create mailbox %s\n", mbox);
        break;
      }
      /* bail out if command failed for reasons other than nonexistent target */
      if (mutt_str_strncasecmp(imap_get_qualifier(idata->buf), "[TRYCREATE]", 11) != 0)
        break;
      mutt_debug(3, "server suggests TRYCREATE\n");
      snprintf(prompt, sizeof(prompt), _("Create %s?"), mbox);
      if (Confirmcreate && mutt_yesorno(prompt, 1) != MUTT_YES)
      {
        mutt_clear_error();
        goto out;
      }
      if (imap_create_mailbox(idata, mbox) < 0)
        break;
      triedcreate = 1;
    }
  } while (rc == -2);

  if (rc != 0)
  {
    imap_error("imap_copy_messages", idata->buf);
    goto out;
  }

  /* cleanup */
  if (delete)
  {
    if (!h)
    {
      for (int i = 0; i < ctx->msgcount; i++)
      {
        if (!message_is_tagged(ctx, i))
          continue;

        mutt_set_flag(ctx, ctx->hdrs[i], MUTT_DELETE, 1);
        mutt_set_flag(ctx, ctx->hdrs[i], MUTT_PURGE, 1);
        if (DeleteUntag)
          mutt_set_flag(ctx, ctx->hdrs[i], MUTT_TAG, 0);
      }
    }
    else
    {
      mutt_set_flag(ctx, h, MUTT_DELETE, 1);
      mutt_set_flag(ctx, h, MUTT_PURGE, 1);
      if (DeleteUntag)
        mutt_set_flag(ctx, h, MUTT_TAG, 0);
    }
  }

  rc = 0;

out:
  if (cmd.data)
    FREE(&cmd.data);
  if (sync_cmd.data)
    FREE(&sync_cmd.data);
  FREE(&mx.mbox);

  return (rc < 0) ? -1 : rc;
}
