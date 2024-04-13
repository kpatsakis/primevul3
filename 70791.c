conv_ext0be(const UChar* s, const UChar* end, UChar* conv)
{
  while (s < end) {
    *conv++ = '\0';
    *conv++ = *s++;
  }
}
