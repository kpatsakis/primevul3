bool QPDecode(const std::string& input, std::string* output) {
  std::string temp;
  temp.reserve(input.size());
  std::string::const_iterator it = input.begin();
  while (it != input.end()) {
    if (*it == '_') {
      temp.push_back(' ');
    } else if (*it == '=') {
      if (input.end() - it < 3) {
        return false;
      }
      if (IsHexDigit(static_cast<unsigned char>(*(it + 1))) &&
          IsHexDigit(static_cast<unsigned char>(*(it + 2)))) {
        unsigned char ch = HexToInt(*(it + 1)) * 16 + HexToInt(*(it + 2));
        temp.push_back(static_cast<char>(ch));
        ++it;
        ++it;
      } else {
        return false;
      }
    } else if (0x20 < *it && *it < 0x7F) {
      DCHECK(*it != 0x3D && *it != 0x5F && *it != 0x3F);
      temp.push_back(*it);
    } else {
      return false;
    }
    ++it;
  }
  output->swap(temp);
  return true;
}
