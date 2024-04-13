int LoadEXR(float **out_rgba, int *width, int *height, const char *filename,
            const char **err) {
  if (out_rgba == NULL) {
    tinyexr::SetErrorMessage("Invalid argument for LoadEXR()", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  EXRVersion exr_version;
  EXRImage exr_image;
  EXRHeader exr_header;
  InitEXRHeader(&exr_header);
  InitEXRImage(&exr_image);

  {
    int ret = ParseEXRVersionFromFile(&exr_version, filename);
    if (ret != TINYEXR_SUCCESS) {
      tinyexr::SetErrorMessage("Invalid EXR header.", err);
      return ret;
    }

    if (exr_version.multipart || exr_version.non_image) {
      tinyexr::SetErrorMessage(
          "Loading multipart or DeepImage is not supported  in LoadEXR() API",
          err);
      return TINYEXR_ERROR_INVALID_DATA;  // @fixme.
    }
  }

  {
    int ret = ParseEXRHeaderFromFile(&exr_header, &exr_version, filename, err);
    if (ret != TINYEXR_SUCCESS) {
      FreeEXRHeader(&exr_header);
      return ret;
    }
  }

  // Read HALF channel as FLOAT.
  for (int i = 0; i < exr_header.num_channels; i++) {
    if (exr_header.pixel_types[i] == TINYEXR_PIXELTYPE_HALF) {
      exr_header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;
    }
  }

  {
    int ret = LoadEXRImageFromFile(&exr_image, &exr_header, filename, err);
    if (ret != TINYEXR_SUCCESS) {
      FreeEXRHeader(&exr_header);
      return ret;
    }
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
    // Assume RGB(A)

    if (idxR == -1) {
      tinyexr::SetErrorMessage("R channel not found", err);

      // @todo { free exr_image }
      FreeEXRHeader(&exr_header);
      return TINYEXR_ERROR_INVALID_DATA;
    }

    if (idxG == -1) {
      tinyexr::SetErrorMessage("G channel not found", err);
      // @todo { free exr_image }
      FreeEXRHeader(&exr_header);
      return TINYEXR_ERROR_INVALID_DATA;
    }

    if (idxB == -1) {
      tinyexr::SetErrorMessage("B channel not found", err);
      // @todo { free exr_image }
      FreeEXRHeader(&exr_header);
      return TINYEXR_ERROR_INVALID_DATA;
    }

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