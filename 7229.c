size_t SaveEXRImageToMemory(const EXRImage *exr_image,
                            const EXRHeader *exr_header,
                            unsigned char **memory_out, const char **err) {
  if (exr_image == NULL || memory_out == NULL ||
      exr_header->compression_type < 0) {
    tinyexr::SetErrorMessage("Invalid argument for SaveEXRImageToMemory", err);
    return 0;
  }

#if !TINYEXR_USE_PIZ
  if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_PIZ) {
    tinyexr::SetErrorMessage("PIZ compression is not supported in this build",
                             err);
    return 0;
  }
#endif

#if !TINYEXR_USE_ZFP
  if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZFP) {
    tinyexr::SetErrorMessage("ZFP compression is not supported in this build",
                             err);
    return 0;
  }
#endif

#if TINYEXR_USE_ZFP
  for (size_t i = 0; i < static_cast<size_t>(exr_header->num_channels); i++) {
    if (exr_header->requested_pixel_types[i] != TINYEXR_PIXELTYPE_FLOAT) {
      tinyexr::SetErrorMessage("Pixel type must be FLOAT for ZFP compression",
                               err);
      return 0;
    }
  }
#endif

  std::vector<unsigned char> memory;

  // Header
  {
    const char header[] = {0x76, 0x2f, 0x31, 0x01};
    memory.insert(memory.end(), header, header + 4);
  }

  // Version, scanline.
  {
    char marker[] = {2, 0, 0, 0};
    /* @todo
    if (exr_header->tiled) {
      marker[1] |= 0x2;
    }
    if (exr_header->long_name) {
      marker[1] |= 0x4;
    }
    if (exr_header->non_image) {
      marker[1] |= 0x8;
    }
    if (exr_header->multipart) {
      marker[1] |= 0x10;
    }
    */
    memory.insert(memory.end(), marker, marker + 4);
  }

  int num_scanlines = 1;
  if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZIP) {
    num_scanlines = 16;
  } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_PIZ) {
    num_scanlines = 32;
  } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZFP) {
    num_scanlines = 16;
  }

  // Write attributes.
  std::vector<tinyexr::ChannelInfo> channels;
  {
    std::vector<unsigned char> data;

    for (int c = 0; c < exr_header->num_channels; c++) {
      tinyexr::ChannelInfo info;
      info.p_linear = 0;
      info.pixel_type = exr_header->requested_pixel_types[c];
      info.x_sampling = 1;
      info.y_sampling = 1;
      info.name = std::string(exr_header->channels[c].name);
      channels.push_back(info);
    }

    tinyexr::WriteChannelInfo(data, channels);

    tinyexr::WriteAttributeToMemory(&memory, "channels", "chlist", &data.at(0),
                                    static_cast<int>(data.size()));
  }

  {
    int comp = exr_header->compression_type;
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&comp));
    tinyexr::WriteAttributeToMemory(
        &memory, "compression", "compression",
        reinterpret_cast<const unsigned char *>(&comp), 1);
  }

  {
    int data[4] = {0, 0, exr_image->width - 1, exr_image->height - 1};
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&data[0]));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&data[1]));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&data[2]));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&data[3]));
    tinyexr::WriteAttributeToMemory(
        &memory, "dataWindow", "box2i",
        reinterpret_cast<const unsigned char *>(data), sizeof(int) * 4);
    tinyexr::WriteAttributeToMemory(
        &memory, "displayWindow", "box2i",
        reinterpret_cast<const unsigned char *>(data), sizeof(int) * 4);
  }

  {
    unsigned char line_order = 0;  // @fixme { read line_order from EXRHeader }
    tinyexr::WriteAttributeToMemory(&memory, "lineOrder", "lineOrder",
                                    &line_order, 1);
  }

  {
    float aspectRatio = 1.0f;
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&aspectRatio));
    tinyexr::WriteAttributeToMemory(
        &memory, "pixelAspectRatio", "float",
        reinterpret_cast<const unsigned char *>(&aspectRatio), sizeof(float));
  }

  {
    float center[2] = {0.0f, 0.0f};
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&center[0]));
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&center[1]));
    tinyexr::WriteAttributeToMemory(
        &memory, "screenWindowCenter", "v2f",
        reinterpret_cast<const unsigned char *>(center), 2 * sizeof(float));
  }

  {
    float w = static_cast<float>(exr_image->width);
    tinyexr::swap4(reinterpret_cast<unsigned int *>(&w));
    tinyexr::WriteAttributeToMemory(&memory, "screenWindowWidth", "float",
                                    reinterpret_cast<const unsigned char *>(&w),
                                    sizeof(float));
  }

  // Custom attributes
  if (exr_header->num_custom_attributes > 0) {
    for (int i = 0; i < exr_header->num_custom_attributes; i++) {
      tinyexr::WriteAttributeToMemory(
          &memory, exr_header->custom_attributes[i].name,
          exr_header->custom_attributes[i].type,
          reinterpret_cast<const unsigned char *>(
              exr_header->custom_attributes[i].value),
          exr_header->custom_attributes[i].size);
    }
  }

  {  // end of header
    unsigned char e = 0;
    memory.push_back(e);
  }

  int num_blocks = exr_image->height / num_scanlines;
  if (num_blocks * num_scanlines < exr_image->height) {
    num_blocks++;
  }

  std::vector<tinyexr::tinyexr_uint64> offsets(static_cast<size_t>(num_blocks));

  size_t headerSize = memory.size();
  tinyexr::tinyexr_uint64 offset =
      headerSize +
      static_cast<size_t>(num_blocks) *
          sizeof(
              tinyexr::tinyexr_int64);  // sizeof(header) + sizeof(offsetTable)

  std::vector<std::vector<unsigned char> > data_list(
      static_cast<size_t>(num_blocks));
  std::vector<size_t> channel_offset_list(
      static_cast<size_t>(exr_header->num_channels));

  int pixel_data_size = 0;
  size_t channel_offset = 0;
  for (size_t c = 0; c < static_cast<size_t>(exr_header->num_channels); c++) {
    channel_offset_list[c] = channel_offset;
    if (exr_header->requested_pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
      pixel_data_size += sizeof(unsigned short);
      channel_offset += sizeof(unsigned short);
    } else if (exr_header->requested_pixel_types[c] ==
               TINYEXR_PIXELTYPE_FLOAT) {
      pixel_data_size += sizeof(float);
      channel_offset += sizeof(float);
    } else if (exr_header->requested_pixel_types[c] == TINYEXR_PIXELTYPE_UINT) {
      pixel_data_size += sizeof(unsigned int);
      channel_offset += sizeof(unsigned int);
    } else {
      assert(0);
    }
  }

#if TINYEXR_USE_ZFP
  tinyexr::ZFPCompressionParam zfp_compression_param;

  // Use ZFP compression parameter from custom attributes(if such a parameter
  // exists)
  {
    bool ret = tinyexr::FindZFPCompressionParam(
        &zfp_compression_param, exr_header->custom_attributes,
        exr_header->num_custom_attributes);

    if (!ret) {
      // Use predefined compression parameter.
      zfp_compression_param.type = 0;
      zfp_compression_param.rate = 2;
    }
  }
#endif

// Use signed int since some OpenMP compiler doesn't allow unsigned type for
// `parallel for`
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (int i = 0; i < num_blocks; i++) {
    size_t ii = static_cast<size_t>(i);
    int start_y = num_scanlines * i;
    int endY = (std::min)(num_scanlines * (i + 1), exr_image->height);
    int h = endY - start_y;

    std::vector<unsigned char> buf(
        static_cast<size_t>(exr_image->width * h * pixel_data_size));

    for (size_t c = 0; c < static_cast<size_t>(exr_header->num_channels); c++) {
      if (exr_header->pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
        if (exr_header->requested_pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT) {
          for (int y = 0; y < h; y++) {
            // Assume increasing Y
            float *line_ptr = reinterpret_cast<float *>(&buf.at(
                static_cast<size_t>(pixel_data_size * y * exr_image->width) +
                channel_offset_list[c] *
                    static_cast<size_t>(exr_image->width)));
            for (int x = 0; x < exr_image->width; x++) {
              tinyexr::FP16 h16;
              h16.u = reinterpret_cast<unsigned short **>(
                  exr_image->images)[c][(y + start_y) * exr_image->width + x];

              tinyexr::FP32 f32 = half_to_float(h16);

              tinyexr::swap4(reinterpret_cast<unsigned int *>(&f32.f));

              // line_ptr[x] = f32.f;
              tinyexr::cpy4(line_ptr + x, &(f32.f));
            }
          }
        } else if (exr_header->requested_pixel_types[c] ==
                   TINYEXR_PIXELTYPE_HALF) {
          for (int y = 0; y < h; y++) {
            // Assume increasing Y
            unsigned short *line_ptr = reinterpret_cast<unsigned short *>(
                &buf.at(static_cast<size_t>(pixel_data_size * y *
                                            exr_image->width) +
                        channel_offset_list[c] *
                            static_cast<size_t>(exr_image->width)));
            for (int x = 0; x < exr_image->width; x++) {
              unsigned short val = reinterpret_cast<unsigned short **>(
                  exr_image->images)[c][(y + start_y) * exr_image->width + x];

              tinyexr::swap2(&val);

              // line_ptr[x] = val;
              tinyexr::cpy2(line_ptr + x, &val);
            }
          }
        } else {
          assert(0);
        }

      } else if (exr_header->pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT) {
        if (exr_header->requested_pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
          for (int y = 0; y < h; y++) {
            // Assume increasing Y
            unsigned short *line_ptr = reinterpret_cast<unsigned short *>(
                &buf.at(static_cast<size_t>(pixel_data_size * y *
                                            exr_image->width) +
                        channel_offset_list[c] *
                            static_cast<size_t>(exr_image->width)));
            for (int x = 0; x < exr_image->width; x++) {
              tinyexr::FP32 f32;
              f32.f = reinterpret_cast<float **>(
                  exr_image->images)[c][(y + start_y) * exr_image->width + x];

              tinyexr::FP16 h16;
              h16 = float_to_half_full(f32);

              tinyexr::swap2(reinterpret_cast<unsigned short *>(&h16.u));

              // line_ptr[x] = h16.u;
              tinyexr::cpy2(line_ptr + x, &(h16.u));
            }
          }
        } else if (exr_header->requested_pixel_types[c] ==
                   TINYEXR_PIXELTYPE_FLOAT) {
          for (int y = 0; y < h; y++) {
            // Assume increasing Y
            float *line_ptr = reinterpret_cast<float *>(&buf.at(
                static_cast<size_t>(pixel_data_size * y * exr_image->width) +
                channel_offset_list[c] *
                    static_cast<size_t>(exr_image->width)));
            for (int x = 0; x < exr_image->width; x++) {
              float val = reinterpret_cast<float **>(
                  exr_image->images)[c][(y + start_y) * exr_image->width + x];

              tinyexr::swap4(reinterpret_cast<unsigned int *>(&val));

              // line_ptr[x] = val;
              tinyexr::cpy4(line_ptr + x, &val);
            }
          }
        } else {
          assert(0);
        }
      } else if (exr_header->pixel_types[c] == TINYEXR_PIXELTYPE_UINT) {
        for (int y = 0; y < h; y++) {
          // Assume increasing Y
          unsigned int *line_ptr = reinterpret_cast<unsigned int *>(&buf.at(
              static_cast<size_t>(pixel_data_size * y * exr_image->width) +
              channel_offset_list[c] * static_cast<size_t>(exr_image->width)));
          for (int x = 0; x < exr_image->width; x++) {
            unsigned int val = reinterpret_cast<unsigned int **>(
                exr_image->images)[c][(y + start_y) * exr_image->width + x];

            tinyexr::swap4(&val);

            // line_ptr[x] = val;
            tinyexr::cpy4(line_ptr + x, &val);
          }
        }
      }
    }

    if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_NONE) {
      // 4 byte: scan line
      // 4 byte: data size
      // ~     : pixel data(uncompressed)
      std::vector<unsigned char> header(8);
      unsigned int data_len = static_cast<unsigned int>(buf.size());
      memcpy(&header.at(0), &start_y, sizeof(int));
      memcpy(&header.at(4), &data_len, sizeof(unsigned int));

      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(0)));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(4)));

      data_list[ii].insert(data_list[ii].end(), header.begin(), header.end());
      data_list[ii].insert(data_list[ii].end(), buf.begin(),
                           buf.begin() + data_len);

    } else if ((exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZIPS) ||
               (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZIP)) {
#if TINYEXR_USE_MINIZ
      std::vector<unsigned char> block(tinyexr::miniz::mz_compressBound(
          static_cast<unsigned long>(buf.size())));
#else
      std::vector<unsigned char> block(
          compressBound(static_cast<uLong>(buf.size())));
#endif
      tinyexr::tinyexr_uint64 outSize = block.size();

      tinyexr::CompressZip(&block.at(0), outSize,
                           reinterpret_cast<const unsigned char *>(&buf.at(0)),
                           static_cast<unsigned long>(buf.size()));

      // 4 byte: scan line
      // 4 byte: data size
      // ~     : pixel data(compressed)
      std::vector<unsigned char> header(8);
      unsigned int data_len = static_cast<unsigned int>(outSize);  // truncate
      memcpy(&header.at(0), &start_y, sizeof(int));
      memcpy(&header.at(4), &data_len, sizeof(unsigned int));

      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(0)));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(4)));

      data_list[ii].insert(data_list[ii].end(), header.begin(), header.end());
      data_list[ii].insert(data_list[ii].end(), block.begin(),
                           block.begin() + data_len);

    } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_RLE) {
      // (buf.size() * 3) / 2 would be enough.
      std::vector<unsigned char> block((buf.size() * 3) / 2);

      tinyexr::tinyexr_uint64 outSize = block.size();

      tinyexr::CompressRle(&block.at(0), outSize,
                           reinterpret_cast<const unsigned char *>(&buf.at(0)),
                           static_cast<unsigned long>(buf.size()));

      // 4 byte: scan line
      // 4 byte: data size
      // ~     : pixel data(compressed)
      std::vector<unsigned char> header(8);
      unsigned int data_len = static_cast<unsigned int>(outSize);  // truncate
      memcpy(&header.at(0), &start_y, sizeof(int));
      memcpy(&header.at(4), &data_len, sizeof(unsigned int));

      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(0)));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(4)));

      data_list[ii].insert(data_list[ii].end(), header.begin(), header.end());
      data_list[ii].insert(data_list[ii].end(), block.begin(),
                           block.begin() + data_len);

    } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_PIZ) {
#if TINYEXR_USE_PIZ
      unsigned int bufLen =
          8192 + static_cast<unsigned int>(
                     2 * static_cast<unsigned int>(
                             buf.size()));  // @fixme { compute good bound. }
      std::vector<unsigned char> block(bufLen);
      unsigned int outSize = static_cast<unsigned int>(block.size());

      CompressPiz(&block.at(0), &outSize,
                  reinterpret_cast<const unsigned char *>(&buf.at(0)),
                  buf.size(), channels, exr_image->width, h);

      // 4 byte: scan line
      // 4 byte: data size
      // ~     : pixel data(compressed)
      std::vector<unsigned char> header(8);
      unsigned int data_len = outSize;
      memcpy(&header.at(0), &start_y, sizeof(int));
      memcpy(&header.at(4), &data_len, sizeof(unsigned int));

      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(0)));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(4)));

      data_list[ii].insert(data_list[ii].end(), header.begin(), header.end());
      data_list[ii].insert(data_list[ii].end(), block.begin(),
                           block.begin() + data_len);

#else
      assert(0);
#endif
    } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZFP) {
#if TINYEXR_USE_ZFP
      std::vector<unsigned char> block;
      unsigned int outSize;

      tinyexr::CompressZfp(
          &block, &outSize, reinterpret_cast<const float *>(&buf.at(0)),
          exr_image->width, h, exr_header->num_channels, zfp_compression_param);

      // 4 byte: scan line
      // 4 byte: data size
      // ~     : pixel data(compressed)
      std::vector<unsigned char> header(8);
      unsigned int data_len = outSize;
      memcpy(&header.at(0), &start_y, sizeof(int));
      memcpy(&header.at(4), &data_len, sizeof(unsigned int));

      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(0)));
      tinyexr::swap4(reinterpret_cast<unsigned int *>(&header.at(4)));

      data_list[ii].insert(data_list[ii].end(), header.begin(), header.end());
      data_list[ii].insert(data_list[ii].end(), block.begin(),
                           block.begin() + data_len);

#else
      assert(0);
#endif
    } else {
      assert(0);
    }
  }  // omp parallel

  for (size_t i = 0; i < static_cast<size_t>(num_blocks); i++) {
    offsets[i] = offset;
    tinyexr::swap8(reinterpret_cast<tinyexr::tinyexr_uint64 *>(&offsets[i]));
    offset += data_list[i].size();
  }

  size_t totalSize = static_cast<size_t>(offset);
  {
    memory.insert(
        memory.end(), reinterpret_cast<unsigned char *>(&offsets.at(0)),
        reinterpret_cast<unsigned char *>(&offsets.at(0)) +
            sizeof(tinyexr::tinyexr_uint64) * static_cast<size_t>(num_blocks));
  }

  if (memory.size() == 0) {
    tinyexr::SetErrorMessage("Output memory size is zero", err);
    return 0;
  }

  (*memory_out) = static_cast<unsigned char *>(malloc(totalSize));
  memcpy((*memory_out), &memory.at(0), memory.size());
  unsigned char *memory_ptr = *memory_out + memory.size();

  for (size_t i = 0; i < static_cast<size_t>(num_blocks); i++) {
    memcpy(memory_ptr, &data_list[i].at(0), data_list[i].size());
    memory_ptr += data_list[i].size();
  }

  return totalSize;  // OK
}