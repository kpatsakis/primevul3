std::string GetResponseHeaderLines(const net::HttpResponseHeaders& headers) {
  std::string raw_headers = headers.raw_headers();
  const char* null_separated_headers = raw_headers.c_str();
  const char* header_line = null_separated_headers;
  std::string cr_separated_headers;
  while (header_line[0] != 0) {
    cr_separated_headers += header_line;
    cr_separated_headers += "\n";
    header_line += strlen(header_line) + 1;
  }
  return cr_separated_headers;
}
