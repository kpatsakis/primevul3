static MagickBooleanType ReadVIPSPixelsNONE(Image *image,
  const VIPSBandFormat format,const VIPSType type,const unsigned int channels,
  ExceptionInfo *exception)
{
  Quantum
    pixel;

  register IndexPacket
    *indexes;

  register PixelPacket
    *q;

  register ssize_t
    x;

  ssize_t
    y;

  for (y = 0; y < (ssize_t) image->rows; y++)
  {
    q=GetAuthenticPixels(image,0,y,image->columns,1,exception);
    if (q == (PixelPacket *) NULL)
      return MagickFalse;
    indexes=GetAuthenticIndexQueue(image);
    for (x=0; x < (ssize_t) image->columns; x++)
    {
      pixel=ReadVIPSPixelNONE(image,format,type);
      SetPixelRed(q,pixel);
      if (channels < 3)
        {
          SetPixelGreen(q,pixel);
          SetPixelBlue(q,pixel);
          if (channels == 2)
            SetPixelAlpha(q,ReadVIPSPixelNONE(image,format,type));
        }
      else
        {
          SetPixelGreen(q,ReadVIPSPixelNONE(image,format,type));
          SetPixelBlue(q,ReadVIPSPixelNONE(image,format,type));
          if (channels == 4)
            {
              if (image->colorspace == CMYKColorspace)
                SetPixelIndex(indexes+x,ReadVIPSPixelNONE(image,format,type))
              else
                SetPixelAlpha(q,ReadVIPSPixelNONE(image,format,type));
            }
          else if (channels == 5)
            {
              SetPixelIndex(indexes+x,ReadVIPSPixelNONE(image,format,type));
              SetPixelAlpha(q,ReadVIPSPixelNONE(image,format,type));
            }
        }
      q++;
    }
    if (SyncAuthenticPixels(image,exception) == MagickFalse)
      return MagickFalse;
  }
  return(MagickTrue);
}
