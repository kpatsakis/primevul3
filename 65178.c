_pango_Is_Emoji_Text_Default (gunichar ch)
{
  return _pango_Is_Emoji (ch) && !_pango_Is_Emoji_Presentation (ch);
}
