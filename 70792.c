 conv_ext0be32(const UChar* s, const UChar* end, UChar* conv)
 {
  while (s < end) {
    *conv++ = '\0';
    *conv++ = '\0';
    *conv++ = '\0';
    *conv++ = *s++;
  }
}
