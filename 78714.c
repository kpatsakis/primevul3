void CopyToLastSlash(const char* spec,
                     int begin,
                     int end,
                     CanonOutput* output) {
  int last_slash = -1;
  for (int i = end - 1; i >= begin; i--) {
    if (spec[i] == '/') {
      last_slash = i;
      break;
    }
  }
  if (last_slash < 0)
    return;  // No slash.

  for (int i = begin; i <= last_slash; i++)
    output->push_back(spec[i]);
}
