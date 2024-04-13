_pango_emoji_iter_init (PangoEmojiIter *iter,
			const char     *text,
			int             length)
{
  iter->text_start = text;
  if (length >= 0)
    iter->text_end = text + length;
  else
    iter->text_end = text + strlen (text);

  iter->start = text;
  iter->end = text;
  iter->is_emoji = (gboolean) 2; /* HACK */

  _pango_emoji_iter_next (iter);

  return iter;
}
