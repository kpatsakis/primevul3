TrimPositions TrimWhitespaceUTF8(const std::string& input,
                                 TrimPositions positions,
                                 std::string* output) {
  DCHECK(IsStringUTF8(input));
  string16 input16 = UTF8ToUTF16(input);
  string16 output16;
  TrimPositions result = TrimWhitespace(input16, positions, &output16);
  *output = UTF16ToUTF8(output16);
  return result;
}
