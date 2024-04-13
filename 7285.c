static int hufCompress(const unsigned short raw[], int nRaw,
                       char compressed[]) {
  if (nRaw == 0) return 0;

  std::vector<long long> freq(HUF_ENCSIZE);

  countFrequencies(freq, raw, nRaw);

  int im = 0;
  int iM = 0;
  hufBuildEncTable(freq.data(), &im, &iM);

  char *tableStart = compressed + 20;
  char *tableEnd = tableStart;
  hufPackEncTable(freq.data(), im, iM, &tableEnd);
  int tableLength = tableEnd - tableStart;

  char *dataStart = tableEnd;
  int nBits = hufEncode(freq.data(), raw, nRaw, iM, dataStart);
  int data_length = (nBits + 7) / 8;

  writeUInt(compressed, im);
  writeUInt(compressed + 4, iM);
  writeUInt(compressed + 8, tableLength);
  writeUInt(compressed + 12, nBits);
  writeUInt(compressed + 16, 0);  // room for future extensions

  return dataStart + data_length - compressed;
}