static void WriteChannelInfo(std::vector<unsigned char> &data,
                             const std::vector<ChannelInfo> &channels) {
  size_t sz = 0;

  // Calculate total size.
  for (size_t c = 0; c < channels.size(); c++) {
    sz += strlen(channels[c].name.c_str()) + 1;  // +1 for \0
    sz += 16;                                    // 4 * int
  }
  data.resize(sz + 1);

  unsigned char *p = &data.at(0);

  for (size_t c = 0; c < channels.size(); c++) {
    memcpy(p, channels[c].name.c_str(), strlen(channels[c].name.c_str()));
    p += strlen(channels[c].name.c_str());
    (*p) = '\0';
    p++;

    int pixel_type = channels[c].pixel_type;
    int x_sampling = channels[c].x_sampling;
    int y_sampling = channels[c].y_sampling;
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&pixel_type));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&x_sampling));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&y_sampling));

    memcpy(p, &pixel_type, sizeof(int));
    p += sizeof(int);

    (*p) = channels[c].p_linear;
    p += 4;

    memcpy(p, &x_sampling, sizeof(int));
    p += sizeof(int);

    memcpy(p, &y_sampling, sizeof(int));
    p += sizeof(int);
  }

  (*p) = '\0';
}