bool IDNToUnicodeOneComponent(const char16* comp,
                              size_t comp_len,
                              const std::wstring& languages,
                              string16* out) {
  DCHECK(out);
  if (comp_len == 0)
    return false;

  static const char16 kIdnPrefix[] = {'x', 'n', '-', '-'};
  if ((comp_len > arraysize(kIdnPrefix)) &&
      !memcmp(comp, kIdnPrefix, arraysize(kIdnPrefix) * sizeof(char16))) {
    size_t original_length = out->length();
    for (int extra_space = 64; ; extra_space *= 2) {
      UErrorCode status = U_ZERO_ERROR;
      out->resize(out->length() + extra_space);
      int output_chars = uidna_IDNToUnicode(comp,
          static_cast<int32_t>(comp_len), &(*out)[original_length], extra_space,
          UIDNA_DEFAULT, NULL, &status);
      if (status == U_ZERO_ERROR) {
        out->resize(original_length + output_chars);
        if (IsIDNComponentSafe(out->data() + original_length, output_chars,
                               languages))
          return true;
      }

      if (status != U_BUFFER_OVERFLOW_ERROR)
        break;
    }
    out->resize(original_length);
  }

  out->append(comp, comp_len);
  return false;
}
