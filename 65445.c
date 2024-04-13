static char *msg_parse_flags(struct ImapHeader *h, char *s)
{
  struct ImapHeaderData *hd = h->data;

  /* sanity-check string */
  if (mutt_str_strncasecmp("FLAGS", s, 5) != 0)
  {
    mutt_debug(1, "not a FLAGS response: %s\n", s);
    return NULL;
  }
  s += 5;
  SKIPWS(s);
  if (*s != '(')
  {
    mutt_debug(1, "bogus FLAGS response: %s\n", s);
    return NULL;
  }
  s++;

  FREE(&hd->flags_system);
  FREE(&hd->flags_remote);

  hd->deleted = hd->flagged = hd->replied = hd->read = hd->old = false;

  /* start parsing */
  while (*s && *s != ')')
  {
    if (mutt_str_strncasecmp("\\deleted", s, 8) == 0)
    {
      s += 8;
      hd->deleted = true;
    }
    else if (mutt_str_strncasecmp("\\flagged", s, 8) == 0)
    {
      s += 8;
      hd->flagged = true;
    }
    else if (mutt_str_strncasecmp("\\answered", s, 9) == 0)
    {
      s += 9;
      hd->replied = true;
    }
    else if (mutt_str_strncasecmp("\\seen", s, 5) == 0)
    {
      s += 5;
      hd->read = true;
    }
    else if (mutt_str_strncasecmp("\\recent", s, 7) == 0)
      s += 7;
    else if (mutt_str_strncasecmp("old", s, 3) == 0)
    {
      s += 3;
      hd->old = MarkOld ? true : false;
    }
    else
    {
      char ctmp;
      char *flag_word = s;
      bool is_system_keyword = (mutt_str_strncasecmp("\\", s, 1) == 0);

      while (*s && !ISSPACE(*s) && *s != ')')
        s++;

      ctmp = *s;
      *s = '\0';

      /* store other system flags as well (mainly \\Draft) */
      if (is_system_keyword)
        mutt_str_append_item(&hd->flags_system, flag_word, ' ');
      /* store custom flags as well */
      else
        mutt_str_append_item(&hd->flags_remote, flag_word, ' ');

      *s = ctmp;
    }
    SKIPWS(s);
  }

  /* wrap up, or note bad flags response */
  if (*s == ')')
    s++;
  else
  {
    mutt_debug(1, "Unterminated FLAGS response: %s\n", s);
    return NULL;
  }

  return s;
}
