conv_encoding(OnigEncoding from, OnigEncoding to, const UChar* s, const UChar* end,
              UChar** conv, UChar** conv_end)
{
  int len = (int )(end - s);

  if (to == ONIG_ENCODING_UTF16_BE) {
    if (from == ONIG_ENCODING_ASCII || from == ONIG_ENCODING_ISO_8859_1) {
      *conv = (UChar* )xmalloc(len * 2);
      CHECK_NULL_RETURN_MEMERR(*conv);
      *conv_end = *conv + (len * 2);
      conv_ext0be(s, end, *conv);
      return 0;
    }
    else if (from == ONIG_ENCODING_UTF16_LE) {
    swap16:
      *conv = (UChar* )xmalloc(len);
      CHECK_NULL_RETURN_MEMERR(*conv);
      *conv_end = *conv + len;
      conv_swap2bytes(s, end, *conv);
      return 0;
    }
  }
  else if (to == ONIG_ENCODING_UTF16_LE) {
    if (from == ONIG_ENCODING_ASCII || from == ONIG_ENCODING_ISO_8859_1) {
      *conv = (UChar* )xmalloc(len * 2);
      CHECK_NULL_RETURN_MEMERR(*conv);
      *conv_end = *conv + (len * 2);
      conv_ext0le(s, end, *conv);
      return 0;
    }
    else if (from == ONIG_ENCODING_UTF16_BE) {
      goto swap16;
    }
  }
  if (to == ONIG_ENCODING_UTF32_BE) {
    if (from == ONIG_ENCODING_ASCII || from == ONIG_ENCODING_ISO_8859_1) {
      *conv = (UChar* )xmalloc(len * 4);
      CHECK_NULL_RETURN_MEMERR(*conv);
      *conv_end = *conv + (len * 4);
      conv_ext0be32(s, end, *conv);
      return 0;
    }
    else if (from == ONIG_ENCODING_UTF32_LE) {
    swap32:
      *conv = (UChar* )xmalloc(len);
      CHECK_NULL_RETURN_MEMERR(*conv);
      *conv_end = *conv + len;
      conv_swap4bytes(s, end, *conv);
      return 0;
    }
  }
  else if (to == ONIG_ENCODING_UTF32_LE) {
    if (from == ONIG_ENCODING_ASCII || from == ONIG_ENCODING_ISO_8859_1) {
      *conv = (UChar* )xmalloc(len * 4);
      CHECK_NULL_RETURN_MEMERR(*conv);
      *conv_end = *conv + (len * 4);
      conv_ext0le32(s, end, *conv);
      return 0;
    }
    else if (from == ONIG_ENCODING_UTF32_BE) {
      goto swap32;
    }
  }
 
   return ONIGERR_NOT_SUPPORTED_ENCODING_COMBINATION;
 }
