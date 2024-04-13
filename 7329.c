static void countFrequencies(std::vector<long long> &freq,
                             const unsigned short data[/*n*/], int n) {
  for (int i = 0; i < HUF_ENCSIZE; ++i) freq[i] = 0;

  for (int i = 0; i < n; ++i) ++freq[data[i]];
}