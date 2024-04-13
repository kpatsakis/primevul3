int LoadEXRFromMemory(float **out_rgba, int *width, int *height,
                      const unsigned char *memory, size_t size,
                      const char **err) {
  if (out_rgba == NULL || memory == NULL) {
    tinyexr::SetErrorMessage("Invalid argument for LoadEXRFromMemory", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  EXRVersion exr_version;
  EXRImage exr_image;
  EXRHeader exr_header;

  InitEXRHeader(&exr_header);

  int ret = ParseEXRVersionFromMemory(&exr_version, memory, size);
  if (ret != TINYEXR_SUCCESS) {
    tinyexr::SetErrorMessage("Failed to parse EXR version", err);
    return ret;
  }

  ret = ParseEXRHeaderFromMemory(&exr_header, &exr_version, memory, size, err);
  if (ret != TINYEXR_SUCCESS) {
    return ret;
  }

  // Read HALF channel as FLOAT.
  for (int i = 0; i < exr_header.num_channels; i++) {
    if (exr_header.pixel_types[i] == TINYEXR_PIXELTYPE_HALF) {
      exr_header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;
    }
  }

  InitEXRImage(&exr_image);
  ret = LoadEXRImageFromMemory(&exr_image, &exr_header, memory, size, err);
  if (ret != TINYEXR_SUCCESS) {
    return ret;
  }

  // RGBA
  int idxR = -1;
  int idxG = -1;
  int idxB = -1;
  int idxA = -1;
  for (int c = 0; c < exr_header.num_channels; c++) {
    if (strcmp(exr_header.channels[c].name, "R") == 0) {
      idxR = c;
    } else if (strcmp(exr_header.channels[c].name, "G") == 0) {
      idxG = c;
    } else if (strcmp(exr_header.channels[c].name, "B") == 0) {
      idxB = c;
    } else if (strcmp(exr_header.channels[c].name, "A") == 0) {
      idxA = c;
    }
  }

  // TODO(syoyo): Refactor removing same code as used in LoadEXR().
  if (exr_header.num_channels == 1) {
    // Grayscale channel only.

    (*out_rgba) = reinterpret_cast<float *>(
        malloc(4 * sizeof(float) * static_cast<size_t>(exr_image.width) *
               static_cast<size_t>(exr_image.height)));

    if (exr_header.tiled) {
      for (int it = 0; it < exr_image.num_tiles; it++) {
        for (int j = 0; j < exr_header.tile_size_y; j++) {
          for (int i = 0; i < exr_header.tile_size_x; i++) {
            const int ii =
                exr_image.tiles[it].offset_x * exr_header.tile_size_x + i;
            const int jj =
                exr_image.tiles[it].offset_y * exr_header.tile_size_y + j;
            const int idx = ii + jj * exr_image.width;

            // out of region check.
            if (ii >= exr_image.width) {
              continue;
            }
            if (jj >= exr_image.height) {
              continue;
            }
            const int srcIdx = i + j * exr_header.tile_size_x;
            unsigned char **src = exr_image.tiles[it].images;
            (*out_rgba)[4 * idx + 0] =
                reinterpret_cast<float **>(src)[0][srcIdx];
            (*out_rgba)[4 * idx + 1] =
                reinterpret_cast<float **>(src)[0][srcIdx];
            (*out_rgba)[4 * idx + 2] =
                reinterpret_cast<float **>(src)[0][srcIdx];
            (*out_rgba)[4 * idx + 3] =
                reinterpret_cast<float **>(src)[0][srcIdx];
          }
        }
      }
    } else {
      for (int i = 0; i < exr_image.width * exr_image.height; i++) {
        const float val = reinterpret_cast<float **>(exr_image.images)[0][i];
        (*out_rgba)[4 * i + 0] = val;
        (*out_rgba)[4 * i + 1] = val;
        (*out_rgba)[4 * i + 2] = val;
        (*out_rgba)[4 * i + 3] = val;
      }
    }

  } else {
    // TODO(syoyo): Support non RGBA image.

    if (idxR == -1) {
      tinyexr::SetErrorMessage("R channel not found", err);

      // @todo { free exr_image }
      return TINYEXR_ERROR_INVALID_DATA;
    }

    if (idxG == -1) {
      tinyexr::SetErrorMessage("G channel not found", err);
      // @todo { free exr_image }
      return TINYEXR_ERROR_INVALID_DATA;
    }

    if (idxB == -1) {
      tinyexr::SetErrorMessage("B channel not found", err);
      // @todo { free exr_image }
      return TINYEXR_ERROR_INVALID_DATA;
    }

    (*out_rgba) = reinterpret_cast<float *>(
        malloc(4 * sizeof(float) * static_cast<size_t>(exr_image.width) *
               static_cast<size_t>(exr_image.height)));

    if (exr_header.tiled) {
      for (int it = 0; it < exr_image.num_tiles; it++) {
        for (int j = 0; j < exr_header.tile_size_y; j++)
          for (int i = 0; i < exr_header.tile_size_x; i++) {
            const int ii =
                exr_image.tiles[it].offset_x * exr_header.tile_size_x + i;
            const int jj =
                exr_image.tiles[it].offset_y * exr_header.tile_size_y + j;
            const int idx = ii + jj * exr_image.width;

            // out of region check.
            if (ii >= exr_image.width) {
              continue;
            }
            if (jj >= exr_image.height) {
              continue;
            }
            const int srcIdx = i + j * exr_header.tile_size_x;
            unsigned char **src = exr_image.tiles[it].images;
            (*out_rgba)[4 * idx + 0] =
                reinterpret_cast<float **>(src)[idxR][srcIdx];
            (*out_rgba)[4 * idx + 1] =
                reinterpret_cast<float **>(src)[idxG][srcIdx];
            (*out_rgba)[4 * idx + 2] =
                reinterpret_cast<float **>(src)[idxB][srcIdx];
            if (idxA != -1) {
              (*out_rgba)[4 * idx + 3] =
                  reinterpret_cast<float **>(src)[idxA][srcIdx];
            } else {
              (*out_rgba)[4 * idx + 3] = 1.0;
            }
          }
      }
    } else {
      for (int i = 0; i < exr_image.width * exr_image.height; i++) {
        (*out_rgba)[4 * i + 0] =
            reinterpret_cast<float **>(exr_image.images)[idxR][i];
        (*out_rgba)[4 * i + 1] =
            reinterpret_cast<float **>(exr_image.images)[idxG][i];
        (*out_rgba)[4 * i + 2] =
            reinterpret_cast<float **>(exr_image.images)[idxB][i];
        if (idxA != -1) {
          (*out_rgba)[4 * i + 3] =
              reinterpret_cast<float **>(exr_image.images)[idxA][i];
        } else {
          (*out_rgba)[4 * i + 3] = 1.0;
        }
      }
    }
  }

  (*width) = exr_image.width;
  (*height) = exr_image.height;

  FreeEXRHeader(&exr_header);
  FreeEXRImage(&exr_image);

  return TINYEXR_SUCCESS;
}