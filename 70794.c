conv_swap2bytes(const UChar* s, const UChar* end, UChar* conv)
{
  while (s < end) {
    *conv++ = s[1];
    *conv++ = s[0];
    s += 2;
  }
}
