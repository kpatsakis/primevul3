static bool ReadChannelInfo(std::vector<ChannelInfo> &channels,
                            const std::vector<unsigned char> &data) {
  const char *p = reinterpret_cast<const char *>(&data.at(0));

  for (;;) {
    if ((*p) == 0) {
      break;
    }
    ChannelInfo info;

    tinyexr_int64 data_len = static_cast<tinyexr_int64>(data.size()) -
                             (p - reinterpret_cast<const char *>(data.data()));
    if (data_len < 0) {
      return false;
    }

    p = ReadString(&info.name, p, size_t(data_len));
    if ((p == NULL) && (info.name.empty())) {
      // Buffer overrun. Issue #51.
      return false;
    }

    const unsigned char *data_end =
        reinterpret_cast<const unsigned char *>(p) + 16;
    if (data_end >= (data.data() + data.size())) {
      return false;
    }

    memcpy(&info.pixel_type, p, sizeof(int));
    p += 4;
    info.p_linear = static_cast<unsigned char>(p[0]);  // uchar
    p += 1 + 3;                                        // reserved: uchar[3]
    memcpy(&info.x_sampling, p, sizeof(int));          // int
    p += 4;
    memcpy(&info.y_sampling, p, sizeof(int));  // int
    p += 4;

    tinyexr::swap4(reinterpret_cast<unsigned int *>(&info.pixel_type));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&info.x_sampling));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&info.y_sampling));

    channels.push_back(info);
  }

  return true;
}