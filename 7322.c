static unsigned char **AllocateImage(int num_channels,
                                     const EXRChannelInfo *channels,
                                     const int *requested_pixel_types,
                                     int data_width, int data_height) {
  unsigned char **images =
      reinterpret_cast<unsigned char **>(static_cast<float **>(
          malloc(sizeof(float *) * static_cast<size_t>(num_channels))));

  for (size_t c = 0; c < static_cast<size_t>(num_channels); c++) {
    size_t data_len =
        static_cast<size_t>(data_width) * static_cast<size_t>(data_height);
    if (channels[c].pixel_type == TINYEXR_PIXELTYPE_HALF) {
      // pixel_data_size += sizeof(unsigned short);
      // channel_offset += sizeof(unsigned short);
      // Alloc internal image for half type.
      if (requested_pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
        images[c] =
            reinterpret_cast<unsigned char *>(static_cast<unsigned short *>(
                malloc(sizeof(unsigned short) * data_len)));
      } else if (requested_pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT) {
        images[c] = reinterpret_cast<unsigned char *>(
            static_cast<float *>(malloc(sizeof(float) * data_len)));
      } else {
        assert(0);
      }
    } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT) {
      // pixel_data_size += sizeof(float);
      // channel_offset += sizeof(float);
      images[c] = reinterpret_cast<unsigned char *>(
          static_cast<float *>(malloc(sizeof(float) * data_len)));
    } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_UINT) {
      // pixel_data_size += sizeof(unsigned int);
      // channel_offset += sizeof(unsigned int);
      images[c] = reinterpret_cast<unsigned char *>(
          static_cast<unsigned int *>(malloc(sizeof(unsigned int) * data_len)));
    } else {
      assert(0);
    }
  }

  return images;
}