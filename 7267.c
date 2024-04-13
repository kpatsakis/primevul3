static bool DecodePixelData(/* out */ unsigned char **out_images,
                            const int *requested_pixel_types,
                            const unsigned char *data_ptr, size_t data_len,
                            int compression_type, int line_order, int width,
                            int height, int x_stride, int y, int line_no,
                            int num_lines, size_t pixel_data_size,
                            size_t num_attributes,
                            const EXRAttribute *attributes, size_t num_channels,
                            const EXRChannelInfo *channels,
                            const std::vector<size_t> &channel_offset_list) {
  if (compression_type == TINYEXR_COMPRESSIONTYPE_PIZ) {  // PIZ
#if TINYEXR_USE_PIZ
    if ((width == 0) || (num_lines == 0) || (pixel_data_size == 0)) {
      // Invalid input #90
      return false;
    }

    // Allocate original data size.
    std::vector<unsigned char> outBuf(static_cast<size_t>(
        static_cast<size_t>(width * num_lines) * pixel_data_size));
    size_t tmpBufLen = outBuf.size();

    bool ret = tinyexr::DecompressPiz(
        reinterpret_cast<unsigned char *>(&outBuf.at(0)), data_ptr, tmpBufLen,
        data_len, static_cast<int>(num_channels), channels, width, num_lines);

    if (!ret) {
      return false;
    }

    // For PIZ_COMPRESSION:
    //   pixel sample data for channel 0 for scanline 0
    //   pixel sample data for channel 1 for scanline 0
    //   pixel sample data for channel ... for scanline 0
    //   pixel sample data for channel n for scanline 0
    //   pixel sample data for channel 0 for scanline 1
    //   pixel sample data for channel 1 for scanline 1
    //   pixel sample data for channel ... for scanline 1
    //   pixel sample data for channel n for scanline 1
    //   ...
    for (size_t c = 0; c < static_cast<size_t>(num_channels); c++) {
      if (channels[c].pixel_type == TINYEXR_PIXELTYPE_HALF) {
        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const unsigned short *line_ptr = reinterpret_cast<unsigned short *>(
              &outBuf.at(v * pixel_data_size * static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            FP16 hf;

            // hf.u = line_ptr[u];
            // use `cpy` to avoid unaligned memory access when compiler's
            // optimization is on.
            tinyexr::cpy2(&(hf.u), line_ptr + u);

            tinyexr::swap2(reinterpret_cast<unsigned short *>(&hf.u));

            if (requested_pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
              unsigned short *image =
                  reinterpret_cast<unsigned short **>(out_images)[c];
              if (line_order == 0) {
                image += (static_cast<size_t>(line_no) + v) *
                             static_cast<size_t>(x_stride) +
                         u;
              } else {
                image += static_cast<size_t>(
                             (height - 1 - (line_no + static_cast<int>(v)))) *
                             static_cast<size_t>(x_stride) +
                         u;
              }
              *image = hf.u;
            } else {  // HALF -> FLOAT
              FP32 f32 = half_to_float(hf);
              float *image = reinterpret_cast<float **>(out_images)[c];
              size_t offset = 0;
              if (line_order == 0) {
                offset = (static_cast<size_t>(line_no) + v) *
                             static_cast<size_t>(x_stride) +
                         u;
              } else {
                offset = static_cast<size_t>(
                             (height - 1 - (line_no + static_cast<int>(v)))) *
                             static_cast<size_t>(x_stride) +
                         u;
              }
              image += offset;
              *image = f32.f;
            }
          }
        }
      } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_UINT) {
        assert(requested_pixel_types[c] == TINYEXR_PIXELTYPE_UINT);

        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const unsigned int *line_ptr = reinterpret_cast<unsigned int *>(
              &outBuf.at(v * pixel_data_size * static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            unsigned int val;
            // val = line_ptr[u];
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(&val);

            unsigned int *image =
                reinterpret_cast<unsigned int **>(out_images)[c];
            if (line_order == 0) {
              image += (static_cast<size_t>(line_no) + v) *
                           static_cast<size_t>(x_stride) +
                       u;
            } else {
              image += static_cast<size_t>(
                           (height - 1 - (line_no + static_cast<int>(v)))) *
                           static_cast<size_t>(x_stride) +
                       u;
            }
            *image = val;
          }
        }
      } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT) {
        assert(requested_pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT);
        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const float *line_ptr = reinterpret_cast<float *>(&outBuf.at(
              v * pixel_data_size * static_cast<size_t>(x_stride) +
              channel_offset_list[c] * static_cast<size_t>(x_stride)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            float val;
            // val = line_ptr[u];
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(reinterpret_cast<unsigned int *>(&val));

            float *image = reinterpret_cast<float **>(out_images)[c];
            if (line_order == 0) {
              image += (static_cast<size_t>(line_no) + v) *
                           static_cast<size_t>(x_stride) +
                       u;
            } else {
              image += static_cast<size_t>(
                           (height - 1 - (line_no + static_cast<int>(v)))) *
                           static_cast<size_t>(x_stride) +
                       u;
            }
            *image = val;
          }
        }
      } else {
        assert(0);
      }
    }
#else
    assert(0 && "PIZ is enabled in this build");
    return false;
#endif

  } else if (compression_type == TINYEXR_COMPRESSIONTYPE_ZIPS ||
             compression_type == TINYEXR_COMPRESSIONTYPE_ZIP) {
    // Allocate original data size.
    std::vector<unsigned char> outBuf(static_cast<size_t>(width) *
                                      static_cast<size_t>(num_lines) *
                                      pixel_data_size);

    unsigned long dstLen = static_cast<unsigned long>(outBuf.size());
    assert(dstLen > 0);
    if (!tinyexr::DecompressZip(
            reinterpret_cast<unsigned char *>(&outBuf.at(0)), &dstLen, data_ptr,
            static_cast<unsigned long>(data_len))) {
      return false;
    }

    // For ZIP_COMPRESSION:
    //   pixel sample data for channel 0 for scanline 0
    //   pixel sample data for channel 1 for scanline 0
    //   pixel sample data for channel ... for scanline 0
    //   pixel sample data for channel n for scanline 0
    //   pixel sample data for channel 0 for scanline 1
    //   pixel sample data for channel 1 for scanline 1
    //   pixel sample data for channel ... for scanline 1
    //   pixel sample data for channel n for scanline 1
    //   ...
    for (size_t c = 0; c < static_cast<size_t>(num_channels); c++) {
      if (channels[c].pixel_type == TINYEXR_PIXELTYPE_HALF) {
        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const unsigned short *line_ptr = reinterpret_cast<unsigned short *>(
              &outBuf.at(v * static_cast<size_t>(pixel_data_size) *
                             static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            tinyexr::FP16 hf;

            // hf.u = line_ptr[u];
            tinyexr::cpy2(&(hf.u), line_ptr + u);

            tinyexr::swap2(reinterpret_cast<unsigned short *>(&hf.u));

            if (requested_pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
              unsigned short *image =
                  reinterpret_cast<unsigned short **>(out_images)[c];
              if (line_order == 0) {
                image += (static_cast<size_t>(line_no) + v) *
                             static_cast<size_t>(x_stride) +
                         u;
              } else {
                image += (static_cast<size_t>(height) - 1U -
                          (static_cast<size_t>(line_no) + v)) *
                             static_cast<size_t>(x_stride) +
                         u;
              }
              *image = hf.u;
            } else {  // HALF -> FLOAT
              tinyexr::FP32 f32 = half_to_float(hf);
              float *image = reinterpret_cast<float **>(out_images)[c];
              size_t offset = 0;
              if (line_order == 0) {
                offset = (static_cast<size_t>(line_no) + v) *
                             static_cast<size_t>(x_stride) +
                         u;
              } else {
                offset = (static_cast<size_t>(height) - 1U -
                          (static_cast<size_t>(line_no) + v)) *
                             static_cast<size_t>(x_stride) +
                         u;
              }
              image += offset;

              *image = f32.f;
            }
          }
        }
      } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_UINT) {
        assert(requested_pixel_types[c] == TINYEXR_PIXELTYPE_UINT);

        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const unsigned int *line_ptr = reinterpret_cast<unsigned int *>(
              &outBuf.at(v * pixel_data_size * static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            unsigned int val;
            // val = line_ptr[u];
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(&val);

            unsigned int *image =
                reinterpret_cast<unsigned int **>(out_images)[c];
            if (line_order == 0) {
              image += (static_cast<size_t>(line_no) + v) *
                           static_cast<size_t>(x_stride) +
                       u;
            } else {
              image += (static_cast<size_t>(height) - 1U -
                        (static_cast<size_t>(line_no) + v)) *
                           static_cast<size_t>(x_stride) +
                       u;
            }
            *image = val;
          }
        }
      } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT) {
        assert(requested_pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT);
        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const float *line_ptr = reinterpret_cast<float *>(
              &outBuf.at(v * pixel_data_size * static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            float val;
            // val = line_ptr[u];
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(reinterpret_cast<unsigned int *>(&val));

            float *image = reinterpret_cast<float **>(out_images)[c];
            if (line_order == 0) {
              image += (static_cast<size_t>(line_no) + v) *
                           static_cast<size_t>(x_stride) +
                       u;
            } else {
              image += (static_cast<size_t>(height) - 1U -
                        (static_cast<size_t>(line_no) + v)) *
                           static_cast<size_t>(x_stride) +
                       u;
            }
            *image = val;
          }
        }
      } else {
        assert(0);
        return false;
      }
    }
  } else if (compression_type == TINYEXR_COMPRESSIONTYPE_RLE) {
    // Allocate original data size.
    std::vector<unsigned char> outBuf(static_cast<size_t>(width) *
                                      static_cast<size_t>(num_lines) *
                                      pixel_data_size);

    unsigned long dstLen = static_cast<unsigned long>(outBuf.size());
    if (dstLen == 0) {
      return false;
    }

    if (!tinyexr::DecompressRle(reinterpret_cast<unsigned char *>(&outBuf.at(0)),
                           dstLen, data_ptr,
                           static_cast<unsigned long>(data_len))) {
      return false;
    }

    // For RLE_COMPRESSION:
    //   pixel sample data for channel 0 for scanline 0
    //   pixel sample data for channel 1 for scanline 0
    //   pixel sample data for channel ... for scanline 0
    //   pixel sample data for channel n for scanline 0
    //   pixel sample data for channel 0 for scanline 1
    //   pixel sample data for channel 1 for scanline 1
    //   pixel sample data for channel ... for scanline 1
    //   pixel sample data for channel n for scanline 1
    //   ...
    for (size_t c = 0; c < static_cast<size_t>(num_channels); c++) {
      if (channels[c].pixel_type == TINYEXR_PIXELTYPE_HALF) {
        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const unsigned short *line_ptr = reinterpret_cast<unsigned short *>(
              &outBuf.at(v * static_cast<size_t>(pixel_data_size) *
                             static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            tinyexr::FP16 hf;

            // hf.u = line_ptr[u];
            tinyexr::cpy2(&(hf.u), line_ptr + u);

            tinyexr::swap2(reinterpret_cast<unsigned short *>(&hf.u));

            if (requested_pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
              unsigned short *image =
                  reinterpret_cast<unsigned short **>(out_images)[c];
              if (line_order == 0) {
                image += (static_cast<size_t>(line_no) + v) *
                             static_cast<size_t>(x_stride) +
                         u;
              } else {
                image += (static_cast<size_t>(height) - 1U -
                          (static_cast<size_t>(line_no) + v)) *
                             static_cast<size_t>(x_stride) +
                         u;
              }
              *image = hf.u;
            } else {  // HALF -> FLOAT
              tinyexr::FP32 f32 = half_to_float(hf);
              float *image = reinterpret_cast<float **>(out_images)[c];
              if (line_order == 0) {
                image += (static_cast<size_t>(line_no) + v) *
                             static_cast<size_t>(x_stride) +
                         u;
              } else {
                image += (static_cast<size_t>(height) - 1U -
                          (static_cast<size_t>(line_no) + v)) *
                             static_cast<size_t>(x_stride) +
                         u;
              }
              *image = f32.f;
            }
          }
        }
      } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_UINT) {
        assert(requested_pixel_types[c] == TINYEXR_PIXELTYPE_UINT);

        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const unsigned int *line_ptr = reinterpret_cast<unsigned int *>(
              &outBuf.at(v * pixel_data_size * static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            unsigned int val;
            // val = line_ptr[u];
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(&val);

            unsigned int *image =
                reinterpret_cast<unsigned int **>(out_images)[c];
            if (line_order == 0) {
              image += (static_cast<size_t>(line_no) + v) *
                           static_cast<size_t>(x_stride) +
                       u;
            } else {
              image += (static_cast<size_t>(height) - 1U -
                        (static_cast<size_t>(line_no) + v)) *
                           static_cast<size_t>(x_stride) +
                       u;
            }
            *image = val;
          }
        }
      } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT) {
        assert(requested_pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT);
        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const float *line_ptr = reinterpret_cast<float *>(
              &outBuf.at(v * pixel_data_size * static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            float val;
            // val = line_ptr[u];
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(reinterpret_cast<unsigned int *>(&val));

            float *image = reinterpret_cast<float **>(out_images)[c];
            if (line_order == 0) {
              image += (static_cast<size_t>(line_no) + v) *
                           static_cast<size_t>(x_stride) +
                       u;
            } else {
              image += (static_cast<size_t>(height) - 1U -
                        (static_cast<size_t>(line_no) + v)) *
                           static_cast<size_t>(x_stride) +
                       u;
            }
            *image = val;
          }
        }
      } else {
        assert(0);
        return false;
      }
    }
  } else if (compression_type == TINYEXR_COMPRESSIONTYPE_ZFP) {
#if TINYEXR_USE_ZFP
    tinyexr::ZFPCompressionParam zfp_compression_param;
    if (!FindZFPCompressionParam(&zfp_compression_param, attributes,
                                 num_attributes)) {
      assert(0);
      return false;
    }

    // Allocate original data size.
    std::vector<unsigned char> outBuf(static_cast<size_t>(width) *
                                      static_cast<size_t>(num_lines) *
                                      pixel_data_size);

    unsigned long dstLen = outBuf.size();
    assert(dstLen > 0);
    tinyexr::DecompressZfp(reinterpret_cast<float *>(&outBuf.at(0)), width,
                           num_lines, num_channels, data_ptr,
                           static_cast<unsigned long>(data_len),
                           zfp_compression_param);

    // For ZFP_COMPRESSION:
    //   pixel sample data for channel 0 for scanline 0
    //   pixel sample data for channel 1 for scanline 0
    //   pixel sample data for channel ... for scanline 0
    //   pixel sample data for channel n for scanline 0
    //   pixel sample data for channel 0 for scanline 1
    //   pixel sample data for channel 1 for scanline 1
    //   pixel sample data for channel ... for scanline 1
    //   pixel sample data for channel n for scanline 1
    //   ...
    for (size_t c = 0; c < static_cast<size_t>(num_channels); c++) {
      assert(channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT);
      if (channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT) {
        assert(requested_pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT);
        for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
          const float *line_ptr = reinterpret_cast<float *>(
              &outBuf.at(v * pixel_data_size * static_cast<size_t>(width) +
                         channel_offset_list[c] * static_cast<size_t>(width)));
          for (size_t u = 0; u < static_cast<size_t>(width); u++) {
            float val;
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(reinterpret_cast<unsigned int *>(&val));

            float *image = reinterpret_cast<float **>(out_images)[c];
            if (line_order == 0) {
              image += (static_cast<size_t>(line_no) + v) *
                           static_cast<size_t>(x_stride) +
                       u;
            } else {
              image += (static_cast<size_t>(height) - 1U -
                        (static_cast<size_t>(line_no) + v)) *
                           static_cast<size_t>(x_stride) +
                       u;
            }
            *image = val;
          }
        }
      } else {
        assert(0);
        return false;
      }
    }
#else
    (void)attributes;
    (void)num_attributes;
    (void)num_channels;
    assert(0);
    return false;
#endif
  } else if (compression_type == TINYEXR_COMPRESSIONTYPE_NONE) {
    for (size_t c = 0; c < num_channels; c++) {
      for (size_t v = 0; v < static_cast<size_t>(num_lines); v++) {
        if (channels[c].pixel_type == TINYEXR_PIXELTYPE_HALF) {
          const unsigned short *line_ptr =
              reinterpret_cast<const unsigned short *>(
                  data_ptr + v * pixel_data_size * size_t(width) +
                  channel_offset_list[c] * static_cast<size_t>(width));

          if (requested_pixel_types[c] == TINYEXR_PIXELTYPE_HALF) {
            unsigned short *outLine =
                reinterpret_cast<unsigned short *>(out_images[c]);
            if (line_order == 0) {
              outLine += (size_t(y) + v) * size_t(x_stride);
            } else {
              outLine +=
                  (size_t(height) - 1 - (size_t(y) + v)) * size_t(x_stride);
            }

            for (int u = 0; u < width; u++) {
              tinyexr::FP16 hf;

              // hf.u = line_ptr[u];
              tinyexr::cpy2(&(hf.u), line_ptr + u);

              tinyexr::swap2(reinterpret_cast<unsigned short *>(&hf.u));

              outLine[u] = hf.u;
            }
          } else if (requested_pixel_types[c] == TINYEXR_PIXELTYPE_FLOAT) {
            float *outLine = reinterpret_cast<float *>(out_images[c]);
            if (line_order == 0) {
              outLine += (size_t(y) + v) * size_t(x_stride);
            } else {
              outLine +=
                  (size_t(height) - 1 - (size_t(y) + v)) * size_t(x_stride);
            }

            if (reinterpret_cast<const unsigned char *>(line_ptr + width) >
                (data_ptr + data_len)) {
              // Insufficient data size
              return false;
            }

            for (int u = 0; u < width; u++) {
              tinyexr::FP16 hf;

              // address may not be aliged. use byte-wise copy for safety.#76
              // hf.u = line_ptr[u];
              tinyexr::cpy2(&(hf.u), line_ptr + u);

              tinyexr::swap2(reinterpret_cast<unsigned short *>(&hf.u));

              tinyexr::FP32 f32 = half_to_float(hf);

              outLine[u] = f32.f;
            }
          } else {
            assert(0);
            return false;
          }
        } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_FLOAT) {
          const float *line_ptr = reinterpret_cast<const float *>(
              data_ptr + v * pixel_data_size * size_t(width) +
              channel_offset_list[c] * static_cast<size_t>(width));

          float *outLine = reinterpret_cast<float *>(out_images[c]);
          if (line_order == 0) {
            outLine += (size_t(y) + v) * size_t(x_stride);
          } else {
            outLine +=
                (size_t(height) - 1 - (size_t(y) + v)) * size_t(x_stride);
          }

          if (reinterpret_cast<const unsigned char *>(line_ptr + width) >
              (data_ptr + data_len)) {
            // Insufficient data size
            return false;
          }

          for (int u = 0; u < width; u++) {
            float val;
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(reinterpret_cast<unsigned int *>(&val));

            outLine[u] = val;
          }
        } else if (channels[c].pixel_type == TINYEXR_PIXELTYPE_UINT) {
          const unsigned int *line_ptr = reinterpret_cast<const unsigned int *>(
              data_ptr + v * pixel_data_size * size_t(width) +
              channel_offset_list[c] * static_cast<size_t>(width));

          unsigned int *outLine =
              reinterpret_cast<unsigned int *>(out_images[c]);
          if (line_order == 0) {
            outLine += (size_t(y) + v) * size_t(x_stride);
          } else {
            outLine +=
                (size_t(height) - 1 - (size_t(y) + v)) * size_t(x_stride);
          }

          for (int u = 0; u < width; u++) {
            if (reinterpret_cast<const unsigned char *>(line_ptr + u) >=
                (data_ptr + data_len)) {
              // Corrupsed data?
              return false;
            }

            unsigned int val;
            tinyexr::cpy4(&val, line_ptr + u);

            tinyexr::swap4(reinterpret_cast<unsigned int *>(&val));

            outLine[u] = val;
          }
        }
      }
    }
  }

  return true;
}