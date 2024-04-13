int imap_msg_open(struct Context *ctx, struct Message *msg, int msgno)
{
  struct Envelope *newenv = NULL;
  char buf[LONG_STRING];
  char path[PATH_MAX];
  char *pc = NULL;
  unsigned int bytes;
  struct Progress progressbar;
  unsigned int uid;
  int cacheno;
  struct ImapCache *cache = NULL;
  bool retried = false;
  bool read;
  int rc;

  /* Sam's weird courier server returns an OK response even when FETCH
   * fails. Thanks Sam. */
  bool fetched = false;
  int output_progress;

  struct ImapData *idata = ctx->data;
  struct Header *h = ctx->hdrs[msgno];

  msg->fp = msg_cache_get(idata, h);
  if (msg->fp)
  {
    if (HEADER_DATA(h)->parsed)
      return 0;
    else
      goto parsemsg;
  }

  /* we still do some caching even if imap_cachedir is unset */
  /* see if we already have the message in our cache */
  cacheno = HEADER_DATA(h)->uid % IMAP_CACHE_LEN;
  cache = &idata->cache[cacheno];

  if (cache->path)
  {
    /* don't treat cache errors as fatal, just fall back. */
    if (cache->uid == HEADER_DATA(h)->uid && (msg->fp = fopen(cache->path, "r")))
      return 0;
    else
    {
      unlink(cache->path);
      FREE(&cache->path);
    }
  }

  /* This function is called in a few places after endwin()
   * e.g. mutt_pipe_message(). */
  output_progress = !isendwin();
  if (output_progress)
    mutt_message(_("Fetching message..."));

  msg->fp = msg_cache_put(idata, h);
  if (!msg->fp)
  {
    cache->uid = HEADER_DATA(h)->uid;
    mutt_mktemp(path, sizeof(path));
    cache->path = mutt_str_strdup(path);
    msg->fp = mutt_file_fopen(path, "w+");
    if (!msg->fp)
    {
      FREE(&cache->path);
      return -1;
    }
  }

  /* mark this header as currently inactive so the command handler won't
   * also try to update it. HACK until all this code can be moved into the
   * command handler */
  h->active = false;

  snprintf(buf, sizeof(buf), "UID FETCH %u %s", HEADER_DATA(h)->uid,
           (mutt_bit_isset(idata->capabilities, IMAP4REV1) ?
                (ImapPeek ? "BODY.PEEK[]" : "BODY[]") :
                "RFC822"));

  imap_cmd_start(idata, buf);
  do
  {
    rc = imap_cmd_step(idata);
    if (rc != IMAP_CMD_CONTINUE)
      break;

    pc = idata->buf;
    pc = imap_next_word(pc);
    pc = imap_next_word(pc);

    if (mutt_str_strncasecmp("FETCH", pc, 5) == 0)
    {
      while (*pc)
      {
        pc = imap_next_word(pc);
        if (pc[0] == '(')
          pc++;
        if (mutt_str_strncasecmp("UID", pc, 3) == 0)
        {
          pc = imap_next_word(pc);
          if (mutt_str_atoui(pc, &uid) < 0)
            goto bail;
          if (uid != HEADER_DATA(h)->uid)
          {
            mutt_error(_(
                "The message index is incorrect. Try reopening the mailbox."));
          }
        }
        else if ((mutt_str_strncasecmp("RFC822", pc, 6) == 0) ||
                 (mutt_str_strncasecmp("BODY[]", pc, 6) == 0))
        {
          pc = imap_next_word(pc);
          if (imap_get_literal_count(pc, &bytes) < 0)
          {
            imap_error("imap_msg_open()", buf);
            goto bail;
          }
          if (output_progress)
          {
            mutt_progress_init(&progressbar, _("Fetching message..."),
                               MUTT_PROGRESS_SIZE, NetInc, bytes);
          }
          if (imap_read_literal(msg->fp, idata, bytes,
                                output_progress ? &progressbar : NULL) < 0)
          {
            goto bail;
          }
          /* pick up trailing line */
          rc = imap_cmd_step(idata);
          if (rc != IMAP_CMD_CONTINUE)
            goto bail;
          pc = idata->buf;

          fetched = true;
        }
        /* UW-IMAP will provide a FLAGS update here if the FETCH causes a
         * change (eg from \Unseen to \Seen).
         * Uncommitted changes in neomutt take precedence. If we decide to
         * incrementally update flags later, this won't stop us syncing */
        else if ((mutt_str_strncasecmp("FLAGS", pc, 5) == 0) && !h->changed)
        {
          pc = imap_set_flags(idata, h, pc, NULL);
          if (!pc)
            goto bail;
        }
      }
    }
  } while (rc == IMAP_CMD_CONTINUE);

  /* see comment before command start. */
  h->active = true;

  fflush(msg->fp);
  if (ferror(msg->fp))
  {
    mutt_perror(cache->path);
    goto bail;
  }

  if (rc != IMAP_CMD_OK)
    goto bail;

  if (!fetched || !imap_code(idata->buf))
    goto bail;

  msg_cache_commit(idata, h);

parsemsg:
  /* Update the header information.  Previously, we only downloaded a
   * portion of the headers, those required for the main display.
   */
  rewind(msg->fp);
  /* It may be that the Status header indicates a message is read, but the
   * IMAP server doesn't know the message has been \Seen. So we capture
   * the server's notion of 'read' and if it differs from the message info
   * picked up in mutt_rfc822_read_header, we mark the message (and context
   * changed). Another possibility: ignore Status on IMAP? */
  read = h->read;
  newenv = mutt_rfc822_read_header(msg->fp, h, 0, 0);
  mutt_env_merge(h->env, &newenv);

  /* see above. We want the new status in h->read, so we unset it manually
   * and let mutt_set_flag set it correctly, updating context. */
  if (read != h->read)
  {
    h->read = read;
    mutt_set_flag(ctx, h, MUTT_NEW, read);
  }

  h->lines = 0;
  fgets(buf, sizeof(buf), msg->fp);
  while (!feof(msg->fp))
  {
    h->lines++;
    fgets(buf, sizeof(buf), msg->fp);
  }

  h->content->length = ftell(msg->fp) - h->content->offset;

  mutt_clear_error();
  rewind(msg->fp);
  HEADER_DATA(h)->parsed = true;

  /* retry message parse if cached message is empty */
  if (!retried && ((h->lines == 0) || (h->content->length == 0)))
  {
    imap_cache_del(idata, h);
    retried = true;
    goto parsemsg;
  }

  return 0;

bail:
  mutt_file_fclose(&msg->fp);
  imap_cache_del(idata, h);
  if (cache->path)
  {
    unlink(cache->path);
    FREE(&cache->path);
  }

  return -1;
}
