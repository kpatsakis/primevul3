static bool DecompressPiz(unsigned char *outPtr, const unsigned char *inPtr,
                          size_t tmpBufSize, size_t inLen, int num_channels,
                          const EXRChannelInfo *channels, int data_width,
                          int num_lines) {
  if (inLen == tmpBufSize) {
    // Data is not compressed(Issue 40).
    memcpy(outPtr, inPtr, inLen);
    return true;
  }

  std::vector<unsigned char> bitmap(BITMAP_SIZE);
  unsigned short minNonZero;
  unsigned short maxNonZero;

#if !MINIZ_LITTLE_ENDIAN
  // @todo { PIZ compression on BigEndian architecture. }
  assert(0);
  return false;
#endif

  memset(bitmap.data(), 0, BITMAP_SIZE);

  const unsigned char *ptr = inPtr;
  // minNonZero = *(reinterpret_cast<const unsigned short *>(ptr));
  tinyexr::cpy2(&minNonZero, reinterpret_cast<const unsigned short *>(ptr));
  // maxNonZero = *(reinterpret_cast<const unsigned short *>(ptr + 2));
  tinyexr::cpy2(&maxNonZero, reinterpret_cast<const unsigned short *>(ptr + 2));
  ptr += 4;

  if (maxNonZero >= BITMAP_SIZE) {
    return false;
  }

  if (minNonZero <= maxNonZero) {
    memcpy(reinterpret_cast<char *>(&bitmap[0] + minNonZero), ptr,
           maxNonZero - minNonZero + 1);
    ptr += maxNonZero - minNonZero + 1;
  }

  std::vector<unsigned short> lut(USHORT_RANGE);
  memset(lut.data(), 0, sizeof(unsigned short) * USHORT_RANGE);
  unsigned short maxValue = reverseLutFromBitmap(bitmap.data(), lut.data());

  //
  // Huffman decoding
  //

  int length;

  // length = *(reinterpret_cast<const int *>(ptr));
  tinyexr::cpy4(&length, reinterpret_cast<const int *>(ptr));
  ptr += sizeof(int);

  if (size_t((ptr - inPtr) + length) > inLen) {
    return false;
  }

  std::vector<unsigned short> tmpBuffer(tmpBufSize);
  hufUncompress(reinterpret_cast<const char *>(ptr), length, &tmpBuffer);

  //
  // Wavelet decoding
  //

  std::vector<PIZChannelData> channelData(static_cast<size_t>(num_channels));

  unsigned short *tmpBufferEnd = &tmpBuffer.at(0);

  for (size_t i = 0; i < static_cast<size_t>(num_channels); ++i) {
    const EXRChannelInfo &chan = channels[i];

    size_t pixelSize = sizeof(int);  // UINT and FLOAT
    if (chan.pixel_type == TINYEXR_PIXELTYPE_HALF) {
      pixelSize = sizeof(short);
    }

    channelData[i].start = tmpBufferEnd;
    channelData[i].end = channelData[i].start;
    channelData[i].nx = data_width;
    channelData[i].ny = num_lines;
    // channelData[i].ys = 1;
    channelData[i].size = static_cast<int>(pixelSize / sizeof(short));

    tmpBufferEnd += channelData[i].nx * channelData[i].ny * channelData[i].size;
  }

  for (size_t i = 0; i < channelData.size(); ++i) {
    PIZChannelData &cd = channelData[i];

    for (int j = 0; j < cd.size; ++j) {
      wav2Decode(cd.start + j, cd.nx, cd.size, cd.ny, cd.nx * cd.size,
                 maxValue);
    }
  }

  //
  // Expand the pixel data to their original range
  //

  applyLut(lut.data(), &tmpBuffer.at(0), static_cast<int>(tmpBufSize));

  for (int y = 0; y < num_lines; y++) {
    for (size_t i = 0; i < channelData.size(); ++i) {
      PIZChannelData &cd = channelData[i];

      // if (modp (y, cd.ys) != 0)
      //    continue;

      size_t n = static_cast<size_t>(cd.nx * cd.size);
      memcpy(outPtr, cd.end, static_cast<size_t>(n * sizeof(unsigned short)));
      outPtr += n * sizeof(unsigned short);
      cd.end += n;
    }
  }

  return true;
}