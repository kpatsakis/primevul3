static int ParseEXRHeader(HeaderInfo *info, bool *empty_header,
                          const EXRVersion *version, std::string *err,
                          const unsigned char *buf, size_t size) {
  const char *marker = reinterpret_cast<const char *>(&buf[0]);

  if (empty_header) {
    (*empty_header) = false;
  }

  if (version->multipart) {
    if (size > 0 && marker[0] == '\0') {
      // End of header list.
      if (empty_header) {
        (*empty_header) = true;
      }
      return TINYEXR_SUCCESS;
    }
  }

  // According to the spec, the header of every OpenEXR file must contain at
  // least the following attributes:
  //
  // channels chlist
  // compression compression
  // dataWindow box2i
  // displayWindow box2i
  // lineOrder lineOrder
  // pixelAspectRatio float
  // screenWindowCenter v2f
  // screenWindowWidth float
  bool has_channels = false;
  bool has_compression = false;
  bool has_data_window = false;
  bool has_display_window = false;
  bool has_line_order = false;
  bool has_pixel_aspect_ratio = false;
  bool has_screen_window_center = false;
  bool has_screen_window_width = false;

  info->data_window[0] = 0;
  info->data_window[1] = 0;
  info->data_window[2] = 0;
  info->data_window[3] = 0;
  info->line_order = 0;  // @fixme
  info->display_window[0] = 0;
  info->display_window[1] = 0;
  info->display_window[2] = 0;
  info->display_window[3] = 0;
  info->screen_window_center[0] = 0.0f;
  info->screen_window_center[1] = 0.0f;
  info->screen_window_width = -1.0f;
  info->pixel_aspect_ratio = -1.0f;

  info->tile_size_x = -1;
  info->tile_size_y = -1;
  info->tile_level_mode = -1;
  info->tile_rounding_mode = -1;

  info->attributes.clear();

  // Read attributes
  size_t orig_size = size;
  for (size_t nattr = 0; nattr < TINYEXR_MAX_HEADER_ATTRIBUTES; nattr++) {
    if (0 == size) {
      if (err) {
        (*err) += "Insufficient data size for attributes.\n";
      }
      return TINYEXR_ERROR_INVALID_DATA;
    } else if (marker[0] == '\0') {
      size--;
      break;
    }

    std::string attr_name;
    std::string attr_type;
    std::vector<unsigned char> data;
    size_t marker_size;
    if (!tinyexr::ReadAttribute(&attr_name, &attr_type, &data, &marker_size,
                                marker, size)) {
      if (err) {
        (*err) += "Failed to read attribute.\n";
      }
      return TINYEXR_ERROR_INVALID_DATA;
    }
    marker += marker_size;
    size -= marker_size;

    if (version->tiled && attr_name.compare("tiles") == 0) {
      unsigned int x_size, y_size;
      unsigned char tile_mode;
      assert(data.size() == 9);
      memcpy(&x_size, &data.at(0), sizeof(int));
      memcpy(&y_size, &data.at(4), sizeof(int));
      tile_mode = data[8];
      tinyexr::swap4(&x_size);
      tinyexr::swap4(&y_size);

      info->tile_size_x = static_cast<int>(x_size);
      info->tile_size_y = static_cast<int>(y_size);

      // mode = levelMode + roundingMode * 16
      info->tile_level_mode = tile_mode & 0x3;
      info->tile_rounding_mode = (tile_mode >> 4) & 0x1;

    } else if (attr_name.compare("compression") == 0) {
      bool ok = false;
      if (data[0] < TINYEXR_COMPRESSIONTYPE_PIZ) {
        ok = true;
      }

      if (data[0] == TINYEXR_COMPRESSIONTYPE_PIZ) {
#if TINYEXR_USE_PIZ
        ok = true;
#else
        if (err) {
          (*err) = "PIZ compression is not supported.";
        }
        return TINYEXR_ERROR_UNSUPPORTED_FORMAT;
#endif
      }

      if (data[0] == TINYEXR_COMPRESSIONTYPE_ZFP) {
#if TINYEXR_USE_ZFP
        ok = true;
#else
        if (err) {
          (*err) = "ZFP compression is not supported.";
        }
        return TINYEXR_ERROR_UNSUPPORTED_FORMAT;
#endif
      }

      if (!ok) {
        if (err) {
          (*err) = "Unknown compression type.";
        }
        return TINYEXR_ERROR_UNSUPPORTED_FORMAT;
      }

      info->compression_type = static_cast<int>(data[0]);
      has_compression = true;

    } else if (attr_name.compare("channels") == 0) {
      // name: zero-terminated string, from 1 to 255 bytes long
      // pixel type: int, possible values are: UINT = 0 HALF = 1 FLOAT = 2
      // pLinear: unsigned char, possible values are 0 and 1
      // reserved: three chars, should be zero
      // xSampling: int
      // ySampling: int

      if (!ReadChannelInfo(info->channels, data)) {
        if (err) {
          (*err) += "Failed to parse channel info.\n";
        }
        return TINYEXR_ERROR_INVALID_DATA;
      }

      if (info->channels.size() < 1) {
        if (err) {
          (*err) += "# of channels is zero.\n";
        }
        return TINYEXR_ERROR_INVALID_DATA;
      }

      has_channels = true;

    } else if (attr_name.compare("dataWindow") == 0) {
      if (data.size() >= 16) {
        memcpy(&info->data_window[0], &data.at(0), sizeof(int));
        memcpy(&info->data_window[1], &data.at(4), sizeof(int));
        memcpy(&info->data_window[2], &data.at(8), sizeof(int));
        memcpy(&info->data_window[3], &data.at(12), sizeof(int));
        tinyexr::swap4(reinterpret_cast<unsigned int *>(&info->data_window[0]));
        tinyexr::swap4(reinterpret_cast<unsigned int *>(&info->data_window[1]));
        tinyexr::swap4(reinterpret_cast<unsigned int *>(&info->data_window[2]));
        tinyexr::swap4(reinterpret_cast<unsigned int *>(&info->data_window[3]));
        has_data_window = true;
      }
    } else if (attr_name.compare("displayWindow") == 0) {
      if (data.size() >= 16) {
        memcpy(&info->display_window[0], &data.at(0), sizeof(int));
        memcpy(&info->display_window[1], &data.at(4), sizeof(int));
        memcpy(&info->display_window[2], &data.at(8), sizeof(int));
        memcpy(&info->display_window[3], &data.at(12), sizeof(int));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->display_window[0]));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->display_window[1]));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->display_window[2]));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->display_window[3]));

        has_display_window = true;
      }
    } else if (attr_name.compare("lineOrder") == 0) {
      if (data.size() >= 1) {
        info->line_order = static_cast<int>(data[0]);
        has_line_order = true;
      }
    } else if (attr_name.compare("pixelAspectRatio") == 0) {
      if (data.size() >= sizeof(float)) {
        memcpy(&info->pixel_aspect_ratio, &data.at(0), sizeof(float));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->pixel_aspect_ratio));
        has_pixel_aspect_ratio = true;
      }
    } else if (attr_name.compare("screenWindowCenter") == 0) {
      if (data.size() >= 8) {
        memcpy(&info->screen_window_center[0], &data.at(0), sizeof(float));
        memcpy(&info->screen_window_center[1], &data.at(4), sizeof(float));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->screen_window_center[0]));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->screen_window_center[1]));
        has_screen_window_center = true;
      }
    } else if (attr_name.compare("screenWindowWidth") == 0) {
      if (data.size() >= sizeof(float)) {
        memcpy(&info->screen_window_width, &data.at(0), sizeof(float));
        tinyexr::swap4(
            reinterpret_cast<unsigned int *>(&info->screen_window_width));

        has_screen_window_width = true;
      }
    } else if (attr_name.compare("chunkCount") == 0) {
      if (data.size() >= sizeof(int)) {
        memcpy(&info->chunk_count, &data.at(0), sizeof(int));
        tinyexr::swap4(reinterpret_cast<unsigned int *>(&info->chunk_count));
      }
    } else {
      // Custom attribute(up to TINYEXR_MAX_CUSTOM_ATTRIBUTES)
      if (info->attributes.size() < TINYEXR_MAX_CUSTOM_ATTRIBUTES) {
        EXRAttribute attrib;
#ifdef _MSC_VER
        strncpy_s(attrib.name, attr_name.c_str(), 255);
        strncpy_s(attrib.type, attr_type.c_str(), 255);
#else
        strncpy(attrib.name, attr_name.c_str(), 255);
        strncpy(attrib.type, attr_type.c_str(), 255);
#endif
        attrib.name[255] = '\0';
        attrib.type[255] = '\0';
        attrib.size = static_cast<int>(data.size());
        attrib.value = static_cast<unsigned char *>(malloc(data.size()));
        memcpy(reinterpret_cast<char *>(attrib.value), &data.at(0),
               data.size());
        info->attributes.push_back(attrib);
      }
    }
  }

  // Check if required attributes exist
  {
    std::stringstream ss_err;

    if (!has_compression) {
      ss_err << "\"compression\" attribute not found in the header."
             << std::endl;
    }

    if (!has_channels) {
      ss_err << "\"channels\" attribute not found in the header." << std::endl;
    }

    if (!has_line_order) {
      ss_err << "\"lineOrder\" attribute not found in the header." << std::endl;
    }

    if (!has_display_window) {
      ss_err << "\"displayWindow\" attribute not found in the header."
             << std::endl;
    }

    if (!has_data_window) {
      ss_err << "\"dataWindow\" attribute not found in the header or invalid."
             << std::endl;
    }

    if (!has_pixel_aspect_ratio) {
      ss_err << "\"pixelAspectRatio\" attribute not found in the header."
             << std::endl;
    }

    if (!has_screen_window_width) {
      ss_err << "\"screenWindowWidth\" attribute not found in the header."
             << std::endl;
    }

    if (!has_screen_window_center) {
      ss_err << "\"screenWindowCenter\" attribute not found in the header."
             << std::endl;
    }

    if (!(ss_err.str().empty())) {
      if (err) {
        (*err) += ss_err.str();
      }
      return TINYEXR_ERROR_INVALID_HEADER;
    }
  }

  info->header_len = static_cast<unsigned int>(orig_size - size);

  return TINYEXR_SUCCESS;
}