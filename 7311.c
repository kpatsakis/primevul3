static bool CompressPiz(unsigned char *outPtr, unsigned int *outSize,
                        const unsigned char *inPtr, size_t inSize,
                        const std::vector<ChannelInfo> &channelInfo,
                        int data_width, int num_lines) {
  std::vector<unsigned char> bitmap(BITMAP_SIZE);
  unsigned short minNonZero;
  unsigned short maxNonZero;

#if !MINIZ_LITTLE_ENDIAN
  // @todo { PIZ compression on BigEndian architecture. }
  assert(0);
  return false;
#endif

  // Assume `inSize` is multiple of 2 or 4.
  std::vector<unsigned short> tmpBuffer(inSize / sizeof(unsigned short));

  std::vector<PIZChannelData> channelData(channelInfo.size());
  unsigned short *tmpBufferEnd = &tmpBuffer.at(0);

  for (size_t c = 0; c < channelData.size(); c++) {
    PIZChannelData &cd = channelData[c];

    cd.start = tmpBufferEnd;
    cd.end = cd.start;

    cd.nx = data_width;
    cd.ny = num_lines;
    // cd.ys = c.channel().ySampling;

    size_t pixelSize = sizeof(int);  // UINT and FLOAT
    if (channelInfo[c].pixel_type == TINYEXR_PIXELTYPE_HALF) {
      pixelSize = sizeof(short);
    }

    cd.size = static_cast<int>(pixelSize / sizeof(short));

    tmpBufferEnd += cd.nx * cd.ny * cd.size;
  }

  const unsigned char *ptr = inPtr;
  for (int y = 0; y < num_lines; ++y) {
    for (size_t i = 0; i < channelData.size(); ++i) {
      PIZChannelData &cd = channelData[i];

      // if (modp (y, cd.ys) != 0)
      //    continue;

      size_t n = static_cast<size_t>(cd.nx * cd.size);
      memcpy(cd.end, ptr, n * sizeof(unsigned short));
      ptr += n * sizeof(unsigned short);
      cd.end += n;
    }
  }

  bitmapFromData(&tmpBuffer.at(0), static_cast<int>(tmpBuffer.size()),
                 bitmap.data(), minNonZero, maxNonZero);

  std::vector<unsigned short> lut(USHORT_RANGE);
  unsigned short maxValue = forwardLutFromBitmap(bitmap.data(), lut.data());
  applyLut(lut.data(), &tmpBuffer.at(0), static_cast<int>(tmpBuffer.size()));

  //
  // Store range compression info in _outBuffer
  //

  char *buf = reinterpret_cast<char *>(outPtr);

  memcpy(buf, &minNonZero, sizeof(unsigned short));
  buf += sizeof(unsigned short);
  memcpy(buf, &maxNonZero, sizeof(unsigned short));
  buf += sizeof(unsigned short);

  if (minNonZero <= maxNonZero) {
    memcpy(buf, reinterpret_cast<char *>(&bitmap[0] + minNonZero),
           maxNonZero - minNonZero + 1);
    buf += maxNonZero - minNonZero + 1;
  }

  //
  // Apply wavelet encoding
  //

  for (size_t i = 0; i < channelData.size(); ++i) {
    PIZChannelData &cd = channelData[i];

    for (int j = 0; j < cd.size; ++j) {
      wav2Encode(cd.start + j, cd.nx, cd.size, cd.ny, cd.nx * cd.size,
                 maxValue);
    }
  }

  //
  // Apply Huffman encoding; append the result to _outBuffer
  //

  // length header(4byte), then huff data. Initialize length header with zero,
  // then later fill it by `length`.
  char *lengthPtr = buf;
  int zero = 0;
  memcpy(buf, &zero, sizeof(int));
  buf += sizeof(int);

  int length =
      hufCompress(&tmpBuffer.at(0), static_cast<int>(tmpBuffer.size()), buf);
  memcpy(lengthPtr, &length, sizeof(int));

  (*outSize) = static_cast<unsigned int>(
      (reinterpret_cast<unsigned char *>(buf) - outPtr) +
      static_cast<unsigned int>(length));

  // Use uncompressed data when compressed data is larger than uncompressed.
  // (Issue 40)
  if ((*outSize) >= inSize) {
    (*outSize) = static_cast<unsigned int>(inSize);
    memcpy(outPtr, inPtr, inSize);
  }
  return true;
}