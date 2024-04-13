static unsigned int CRC32(const unsigned char *message,const size_t length)
{
  register ssize_t
    i;

  static MagickBooleanType
    crc_initial = MagickFalse;

  static unsigned int
    crc_xor[256];

  unsigned int
    crc;

  /*
    Generate a 32-bit cyclic redundancy check for the message.
  */
  if (crc_initial == MagickFalse)
    {
      register unsigned int
        i;

      unsigned int
        alpha;

      for (i=0; i < 256; i++)
      {
        register ssize_t
          j;

        alpha=i;
        for (j=0; j < 8; j++)
          alpha=(alpha & 0x01) ? (0xEDB88320 ^ (alpha >> 1)) : (alpha >> 1);
        crc_xor[i]=alpha;
      }
      crc_initial=MagickTrue;
    }
  crc=0xFFFFFFFF;
  for (i=0; i < (ssize_t) length; i++)
    crc=crc_xor[(crc ^ message[i]) & 0xff] ^ (crc >> 8);
  return(crc ^ 0xFFFFFFFF);
}
