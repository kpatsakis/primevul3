static bool ReconstructLineOffsets(
    std::vector<tinyexr::tinyexr_uint64> *offsets, size_t n,
    const unsigned char *head, const unsigned char *marker, const size_t size) {
  assert(head < marker);
  assert(offsets->size() == n);

  for (size_t i = 0; i < n; i++) {
    size_t offset = static_cast<size_t>(marker - head);
    // Offset should not exceed whole EXR file/data size.
    if ((offset + sizeof(tinyexr::tinyexr_uint64)) >= size) {
      return false;
    }

    int y;
    unsigned int data_len;

    memcpy(&y, marker, sizeof(int));
    memcpy(&data_len, marker + 4, sizeof(unsigned int));

    if (data_len >= size) {
      return false;
    }

    tinyexr::swap4(reinterpret_cast<unsigned int *>(&y));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&data_len));

    (*offsets)[i] = offset;

    marker += data_len + 8;  // 8 = 4 bytes(y) + 4 bytes(data_len)
  }

  return true;
}