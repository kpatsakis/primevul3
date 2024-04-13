bool DecodeBQEncoding(const std::string& part, RFC2047EncodingType enc_type,
                       const std::string& charset, std::string* output) {
  std::string decoded;
  if (enc_type == B_ENCODING) {
    if (!base::Base64Decode(part, &decoded)) {
      return false;
    }
  } else {
    if (!QPDecode(part, &decoded)) {
      return false;
    }
  }

  UErrorCode err = U_ZERO_ERROR;
  UConverter* converter(ucnv_open(charset.c_str(), &err));
  if (U_FAILURE(err)) {
    return false;
  }

  int length = static_cast<int>(decoded.length());
  char* buf = WriteInto(output, length * 3);
  length = ucnv_toAlgorithmic(UCNV_UTF8, converter, buf, length * 3,
      decoded.data(), length, &err);
  ucnv_close(converter);
  if (U_FAILURE(err)) {
    return false;
  }
  output->resize(length);
  return true;
}
