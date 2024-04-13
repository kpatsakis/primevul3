conv_ext0le32(const UChar* s, const UChar* end, UChar* conv)
{
  while (s < end) {
    *conv++ = *s++;
    *conv++ = '\0';
    *conv++ = '\0';
    *conv++ = '\0';
  }
}
