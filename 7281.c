static int rleCompress(int inLength, const char in[], signed char out[]) {
  const char *inEnd = in + inLength;
  const char *runStart = in;
  const char *runEnd = in + 1;
  signed char *outWrite = out;

  while (runStart < inEnd) {
    while (runEnd < inEnd && *runStart == *runEnd &&
           runEnd - runStart - 1 < MAX_RUN_LENGTH) {
      ++runEnd;
    }

    if (runEnd - runStart >= MIN_RUN_LENGTH) {
      //
      // Compressable run
      //

      *outWrite++ = static_cast<char>(runEnd - runStart) - 1;
      *outWrite++ = *(reinterpret_cast<const signed char *>(runStart));
      runStart = runEnd;
    } else {
      //
      // Uncompressable run
      //

      while (runEnd < inEnd &&
             ((runEnd + 1 >= inEnd || *runEnd != *(runEnd + 1)) ||
              (runEnd + 2 >= inEnd || *(runEnd + 1) != *(runEnd + 2))) &&
             runEnd - runStart < MAX_RUN_LENGTH) {
        ++runEnd;
      }

      *outWrite++ = static_cast<char>(runStart - runEnd);

      while (runStart < runEnd) {
        *outWrite++ = *(reinterpret_cast<const signed char *>(runStart++));
      }
    }

    ++runEnd;
  }

  return static_cast<int>(outWrite - out);
}