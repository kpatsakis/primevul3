void UTF8PartsToUTF16Parts(const std::string& text_utf8,
                           const url_parse::Parsed& parts_utf8,
                           url_parse::Parsed* parts) {
  if (IsStringASCII(text_utf8)) {
    *parts = parts_utf8;
    return;
  }

  parts->scheme =
      UTF8ComponentToUTF16Component(text_utf8, parts_utf8.scheme);
  parts ->username =
      UTF8ComponentToUTF16Component(text_utf8, parts_utf8.username);
  parts->password =
      UTF8ComponentToUTF16Component(text_utf8, parts_utf8.password);
  parts->host =
      UTF8ComponentToUTF16Component(text_utf8, parts_utf8.host);
  parts->port =
      UTF8ComponentToUTF16Component(text_utf8, parts_utf8.port);
  parts->path =
      UTF8ComponentToUTF16Component(text_utf8, parts_utf8.path);
  parts->query =
      UTF8ComponentToUTF16Component(text_utf8, parts_utf8.query);
  parts->ref =
      UTF8ComponentToUTF16Component(text_utf8, parts_utf8.ref);
}
