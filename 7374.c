static int rleUncompress(int inLength, int maxLength, const signed char in[],
                         char out[]) {
  char *outStart = out;

  while (inLength > 0) {
    if (*in < 0) {
      int count = -(static_cast<int>(*in++));
      inLength -= count + 1;

      // Fixes #116: Add bounds check to in buffer.
      if ((0 > (maxLength -= count)) || (inLength < 0)) return 0;

      memcpy(out, in, count);
      out += count;
      in += count;
    } else {
      int count = *in++;
      inLength -= 2;

      if (0 > (maxLength -= count + 1)) return 0;

      memset(out, *reinterpret_cast<const char *>(in), count + 1);
      out += count + 1;

      in++;
    }
  }

  return static_cast<int>(out - outStart);
}