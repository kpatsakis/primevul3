_pango_Is_Emoji_Keycap_Base (gunichar ch)
{
  return (ch >= '0' && ch <= '9') || ch == '#' || ch == '*';
}
