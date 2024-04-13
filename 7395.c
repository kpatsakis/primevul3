static bool DecompressZfp(float *dst, int dst_width, int dst_num_lines,
                          int num_channels, const unsigned char *src,
                          unsigned long src_size,
                          const ZFPCompressionParam &param) {
  size_t uncompressed_size = dst_width * dst_num_lines * num_channels;

  if (uncompressed_size == src_size) {
    // Data is not compressed(Issue 40).
    memcpy(dst, src, src_size);
  }

  zfp_stream *zfp = NULL;
  zfp_field *field = NULL;

  assert((dst_width % 4) == 0);
  assert((dst_num_lines % 4) == 0);

  if ((dst_width & 3U) || (dst_num_lines & 3U)) {
    return false;
  }

  field =
      zfp_field_2d(reinterpret_cast<void *>(const_cast<unsigned char *>(src)),
                   zfp_type_float, dst_width, dst_num_lines * num_channels);
  zfp = zfp_stream_open(NULL);

  if (param.type == TINYEXR_ZFP_COMPRESSIONTYPE_RATE) {
    zfp_stream_set_rate(zfp, param.rate, zfp_type_float, /* dimention */ 2,
                        /* write random access */ 0);
  } else if (param.type == TINYEXR_ZFP_COMPRESSIONTYPE_PRECISION) {
    zfp_stream_set_precision(zfp, param.precision, zfp_type_float);
  } else if (param.type == TINYEXR_ZFP_COMPRESSIONTYPE_ACCURACY) {
    zfp_stream_set_accuracy(zfp, param.tolerance, zfp_type_float);
  } else {
    assert(0);
  }

  size_t buf_size = zfp_stream_maximum_size(zfp, field);
  std::vector<unsigned char> buf(buf_size);
  memcpy(&buf.at(0), src, src_size);

  bitstream *stream = stream_open(&buf.at(0), buf_size);
  zfp_stream_set_bit_stream(zfp, stream);
  zfp_stream_rewind(zfp);

  size_t image_size = dst_width * dst_num_lines;

  for (int c = 0; c < num_channels; c++) {
    // decompress 4x4 pixel block.
    for (int y = 0; y < dst_num_lines; y += 4) {
      for (int x = 0; x < dst_width; x += 4) {
        float fblock[16];
        zfp_decode_block_float_2(zfp, fblock);
        for (int j = 0; j < 4; j++) {
          for (int i = 0; i < 4; i++) {
            dst[c * image_size + ((y + j) * dst_width + (x + i))] =
                fblock[j * 4 + i];
          }
        }
      }
    }
  }

  zfp_field_free(field);
  zfp_stream_close(zfp);
  stream_close(stream);

  return true;
}