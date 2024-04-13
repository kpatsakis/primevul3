static bool hufUncompress(const char compressed[], int nCompressed,
                          std::vector<unsigned short> *raw) {
  if (nCompressed == 0) {
    if (raw->size() != 0) return false;

    return false;
  }

  int im = readUInt(compressed);
  int iM = readUInt(compressed + 4);
  // int tableLength = readUInt (compressed + 8);
  int nBits = readUInt(compressed + 12);

  if (im < 0 || im >= HUF_ENCSIZE || iM < 0 || iM >= HUF_ENCSIZE) return false;

  const char *ptr = compressed + 20;

  //
  // Fast decoder needs at least 2x64-bits of compressed data, and
  // needs to be run-able on this platform. Otherwise, fall back
  // to the original decoder
  //

  // if (FastHufDecoder::enabled() && nBits > 128)
  //{
  //    FastHufDecoder fhd (ptr, nCompressed - (ptr - compressed), im, iM, iM);
  //    fhd.decode ((unsigned char*)ptr, nBits, raw, nRaw);
  //}
  // else
  {
    std::vector<long long> freq(HUF_ENCSIZE);
    std::vector<HufDec> hdec(HUF_DECSIZE);

    hufClearDecTable(&hdec.at(0));

    hufUnpackEncTable(&ptr, nCompressed - (ptr - compressed), im, iM,
                      &freq.at(0));

    {
      if (nBits > 8 * (nCompressed - (ptr - compressed))) {
        return false;
      }

      hufBuildDecTable(&freq.at(0), im, iM, &hdec.at(0));
      hufDecode(&freq.at(0), &hdec.at(0), ptr, nBits, iM, raw->size(),
                raw->data());
    }
    // catch (...)
    //{
    //    hufFreeDecTable (hdec);
    //    throw;
    //}

    hufFreeDecTable(&hdec.at(0));
  }

  return true;
}