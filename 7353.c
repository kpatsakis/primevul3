static int DecodeEXRImage(EXRImage *exr_image, const EXRHeader *exr_header,
                          const unsigned char *head,
                          const unsigned char *marker, const size_t size,
                          const char **err) {
  if (exr_image == NULL || exr_header == NULL || head == NULL ||
      marker == NULL || (size <= tinyexr::kEXRVersionSize)) {
    tinyexr::SetErrorMessage("Invalid argument for DecodeEXRImage().", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  int num_scanline_blocks = 1;
  if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZIP) {
    num_scanline_blocks = 16;
  } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_PIZ) {
    num_scanline_blocks = 32;
  } else if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZFP) {
    num_scanline_blocks = 16;
  }

  int data_width = exr_header->data_window[2] - exr_header->data_window[0];
  if (data_width >= std::numeric_limits<int>::max()) {
    // Issue 63
    tinyexr::SetErrorMessage("Invalid data width value", err);
    return TINYEXR_ERROR_INVALID_DATA;
  }
  data_width++;

  int data_height = exr_header->data_window[3] - exr_header->data_window[1];
  if (data_height >= std::numeric_limits<int>::max()) {
    tinyexr::SetErrorMessage("Invalid data height value", err);
    return TINYEXR_ERROR_INVALID_DATA;
  }
  data_height++;

  if ((data_width < 0) || (data_height < 0)) {
    tinyexr::SetErrorMessage("data width or data height is negative.", err);
    return TINYEXR_ERROR_INVALID_DATA;
  }

  // Do not allow too large data_width and data_height. header invalid?
  {
    const int threshold = 1024 * 8192;  // heuristics
    if (data_width > threshold) {
      tinyexr::SetErrorMessage("data width too large.", err);
      return TINYEXR_ERROR_INVALID_DATA;
    }
    if (data_height > threshold) {
      tinyexr::SetErrorMessage("data height too large.", err);
      return TINYEXR_ERROR_INVALID_DATA;
    }
  }

  // Read offset tables.
  size_t num_blocks = 0;

  if (exr_header->chunk_count > 0) {
    // Use `chunkCount` attribute.
    num_blocks = static_cast<size_t>(exr_header->chunk_count);
  } else if (exr_header->tiled) {
    // @todo { LoD }
    size_t num_x_tiles = static_cast<size_t>(data_width) /
                         static_cast<size_t>(exr_header->tile_size_x);
    if (num_x_tiles * static_cast<size_t>(exr_header->tile_size_x) <
        static_cast<size_t>(data_width)) {
      num_x_tiles++;
    }
    size_t num_y_tiles = static_cast<size_t>(data_height) /
                         static_cast<size_t>(exr_header->tile_size_y);
    if (num_y_tiles * static_cast<size_t>(exr_header->tile_size_y) <
        static_cast<size_t>(data_height)) {
      num_y_tiles++;
    }

    num_blocks = num_x_tiles * num_y_tiles;
  } else {
    num_blocks = static_cast<size_t>(data_height) /
                 static_cast<size_t>(num_scanline_blocks);
    if (num_blocks * static_cast<size_t>(num_scanline_blocks) <
        static_cast<size_t>(data_height)) {
      num_blocks++;
    }
  }

  std::vector<tinyexr::tinyexr_uint64> offsets(num_blocks);

  for (size_t y = 0; y < num_blocks; y++) {
    tinyexr::tinyexr_uint64 offset;
    // Issue #81
    if ((marker + sizeof(tinyexr_uint64)) >= (head + size)) {
      tinyexr::SetErrorMessage("Insufficient data size in offset table.", err);
      return TINYEXR_ERROR_INVALID_DATA;
    }

    memcpy(&offset, marker, sizeof(tinyexr::tinyexr_uint64));
    tinyexr::swap8(&offset);
    if (offset >= size) {
      tinyexr::SetErrorMessage("Invalid offset value in DecodeEXRImage.", err);
      return TINYEXR_ERROR_INVALID_DATA;
    }
    marker += sizeof(tinyexr::tinyexr_uint64);  // = 8
    offsets[y] = offset;
  }

  // If line offsets are invalid, we try to reconstruct it.
  // See OpenEXR/IlmImf/ImfScanLineInputFile.cpp::readLineOffsets() for details.
  for (size_t y = 0; y < num_blocks; y++) {
    if (offsets[y] <= 0) {
      // TODO(syoyo) Report as warning?
      // if (err) {
      //  stringstream ss;
      //  ss << "Incomplete lineOffsets." << std::endl;
      //  (*err) += ss.str();
      //}
      bool ret =
          ReconstructLineOffsets(&offsets, num_blocks, head, marker, size);
      if (ret) {
        // OK
        break;
      } else {
        tinyexr::SetErrorMessage(
            "Cannot reconstruct lineOffset table in DecodeEXRImage.", err);
        return TINYEXR_ERROR_INVALID_DATA;
      }
    }
  }

  {
    std::string e;
    int ret = DecodeChunk(exr_image, exr_header, offsets, head, size, &e);

    if (ret != TINYEXR_SUCCESS) {
      if (!e.empty()) {
        tinyexr::SetErrorMessage(e, err);
      }

      // release memory(if exists)
      if ((exr_header->num_channels > 0) && exr_image && exr_image->images) {
        for (size_t c = 0; c < size_t(exr_header->num_channels); c++) {
          if (exr_image->images[c]) {
            free(exr_image->images[c]);
            exr_image->images[c] = NULL;
          }
        }
        free(exr_image->images);
        exr_image->images = NULL;
      }
    }

    return ret;
  }
}