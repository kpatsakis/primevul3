static bool ComputeChannelLayout(std::vector<size_t> *channel_offset_list,
                                 int *pixel_data_size, size_t *channel_offset,
                                 int num_channels,
                                 const EXRChannelInfo *channels) {
  channel_offset_list->resize(static_cast<size_t>(num_channels));

  (*pixel_data_size) = 0;
  (*channel_offset) = 0;

  for (size_t c = 0; c < static_cast<size_t>(num_channels); c++) {
    (*channel_offset_list)[c] = (*channel_offset);
    if (channels[c].pixel_type == TINYEXR_PIXELTYPE_HALF) {
      (*pixel_data_size) += sizeof(unsigned short);
      (*channel_offset) += sizeof(unsigned short);
    } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT) {
      (*pixel_data_size) += sizeof(float);
      (*channel_offset) += sizeof(float);
    } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_UINT) {
      (*pixel_data_size) += sizeof(unsigned int);
      (*channel_offset) += sizeof(unsigned int);
    } else {
      // ???
      return false;
    }
  }
  return true;
}