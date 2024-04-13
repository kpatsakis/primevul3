bool IsCompatibleWithASCIILetters(const std::string& lang) {
  return !lang.substr(0, 2).compare("zh") ||
         !lang.substr(0, 2).compare("ja") ||
         !lang.substr(0, 2).compare("ko");
}
