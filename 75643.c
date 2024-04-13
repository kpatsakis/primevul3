static std::string FixupHomedir(const std::string& text) {
  DCHECK(text.length() > 0 && text[0] == '~');

  if (text.length() == 1 || text[1] == '/') {
    const char* home = getenv(base::env_vars::kHome);
    if (URLFixerUpper::home_directory_override)
      home = URLFixerUpper::home_directory_override;
    if (!home)
      return text;
    return home + text.substr(1);
  }


#if defined(OS_MACOSX)
  static const char kHome[] = "/Users/";
#else
  static const char kHome[] = "/home/";
#endif
  return kHome + text.substr(1);
}
