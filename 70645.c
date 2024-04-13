static MagickPixelPacket **DestroyPixelThreadSet(MagickPixelPacket **pixels)
{
  register ssize_t
    i;

  assert(pixels != (MagickPixelPacket **) NULL);
  for (i=0; i < (ssize_t) GetMagickResourceLimit(ThreadResource); i++)
    if (pixels[i] != (MagickPixelPacket *) NULL)
      pixels[i]=(MagickPixelPacket *) RelinquishMagickMemory(pixels[i]);
  pixels=(MagickPixelPacket **) RelinquishMagickMemory(pixels);
  return(pixels);
}
