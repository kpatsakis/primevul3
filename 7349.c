bool CompressZfp(std::vector<unsigned char> *outBuf, unsigned int *outSize,
                 const float *inPtr, int width, int num_lines, int num_channels,
                 const ZFPCompressionParam &param) {
  zfp_stream *zfp = NULL;
  zfp_field *field = NULL;

  assert((width % 4) == 0);
  assert((num_lines % 4) == 0);

  if ((width & 3U) || (num_lines & 3U)) {
    return false;
  }

  // create input array.
  field = zfp_field_2d(reinterpret_cast<void *>(const_cast<float *>(inPtr)),
                       zfp_type_float, width, num_lines * num_channels);

  zfp = zfp_stream_open(NULL);

  if (param.type == TINYEXR_ZFP_COMPRESSIONTYPE_RATE) {
    zfp_stream_set_rate(zfp, param.rate, zfp_type_float, 2, 0);
  } else if (param.type == TINYEXR_ZFP_COMPRESSIONTYPE_PRECISION) {
    zfp_stream_set_precision(zfp, param.precision, zfp_type_float);
  } else if (param.type == TINYEXR_ZFP_COMPRESSIONTYPE_ACCURACY) {
    zfp_stream_set_accuracy(zfp, param.tolerance, zfp_type_float);
  } else {
    assert(0);
  }

  size_t buf_size = zfp_stream_maximum_size(zfp, field);

  outBuf->resize(buf_size);

  bitstream *stream = stream_open(&outBuf->at(0), buf_size);
  zfp_stream_set_bit_stream(zfp, stream);
  zfp_field_free(field);

  size_t image_size = width * num_lines;

  for (int c = 0; c < num_channels; c++) {
    // compress 4x4 pixel block.
    for (int y = 0; y < num_lines; y += 4) {
      for (int x = 0; x < width; x += 4) {
        float fblock[16];
        for (int j = 0; j < 4; j++) {
          for (int i = 0; i < 4; i++) {
            fblock[j * 4 + i] =
                inPtr[c * image_size + ((y + j) * width + (x + i))];
          }
        }
        zfp_encode_block_float_2(zfp, fblock);
      }
    }
  }

  zfp_stream_flush(zfp);
  (*outSize) = zfp_stream_compressed_size(zfp);

  zfp_stream_close(zfp);

  return true;
}