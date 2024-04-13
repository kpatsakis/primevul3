conv_swap4bytes(const UChar* s, const UChar* end, UChar* conv)
{
  while (s < end) {
    *conv++ = s[3];
    *conv++ = s[2];
    *conv++ = s[1];
    *conv++ = s[0];
    s += 4;
  }
}
