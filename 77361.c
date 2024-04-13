std::string ProcessIDN(const std::string& input) {
  string16 input16;
  input16.reserve(input.length());
  input16.insert(input16.end(), input.begin(), input.end());

  string16 output16;
  output16.resize(input.length());

  UErrorCode status = U_ZERO_ERROR;
  int output_chars = uidna_IDNToUnicode(input16.data(), input.length(),
                                        &output16[0], output16.length(),
                                        UIDNA_DEFAULT, NULL, &status);
  if (status == U_ZERO_ERROR) {
    output16.resize(output_chars);
  } else if (status != U_BUFFER_OVERFLOW_ERROR) {
    return input;
  } else {
    output16.resize(output_chars);
    output_chars = uidna_IDNToUnicode(input16.data(), input.length(),
                                      &output16[0], output16.length(),
                                      UIDNA_DEFAULT, NULL, &status);
    if (status != U_ZERO_ERROR)
      return input;
    DCHECK_EQ(static_cast<size_t>(output_chars), output16.length());
    output16.resize(output_chars);  // Just to be safe.
  }

  if (input16 == output16)
    return input;  // Input did not contain any encoded data.

  return l10n_util::GetStringFUTF8(IDS_CERT_INFO_IDN_VALUE_FORMAT,
                                   input16, output16);
}
