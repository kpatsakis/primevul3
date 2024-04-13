static MagickStatusType ReadPSDChannelPixels(Image *image,const size_t channels,
  const size_t row,const ssize_t type,const unsigned char *pixels,
  ExceptionInfo *exception)
{
  Quantum
    pixel;

  register const unsigned char
    *p;

  register IndexPacket
    *indexes;

  register PixelPacket
    *q;

  register ssize_t
    x;

  size_t
    packet_size;

  unsigned short
    nibble;

  p=pixels;
  q=GetAuthenticPixels(image,0,row,image->columns,1,exception);
  if (q == (PixelPacket *) NULL)
    return MagickFalse;
  indexes=GetAuthenticIndexQueue(image);
  packet_size=GetPSDPacketSize(image);
  for (x=0; x < (ssize_t) image->columns; x++)
  {
    if (packet_size == 1)
      pixel=ScaleCharToQuantum(*p++);
    else
      {
        p=PushShortPixel(MSBEndian,p,&nibble);
        pixel=ScaleShortToQuantum(nibble);
      }
    switch (type)
    {
      case -1:
      {
        SetPixelAlpha(q,pixel);
        break;
      }
      case 0:
      {
        SetPixelRed(q,pixel);
        if (channels == 1)
          {
            SetPixelGreen(q,GetPixelRed(q));
            SetPixelBlue(q,GetPixelRed(q));
          }
        if (image->storage_class == PseudoClass)
          {
            if (packet_size == 1)
              SetPixelIndex(indexes+x,ScaleQuantumToChar(pixel))
            else
              SetPixelIndex(indexes+x,ScaleQuantumToShort(pixel))
            SetPixelRGBO(q,image->colormap+(ssize_t)
              ConstrainColormapIndex(image,GetPixelIndex(indexes+x)));
            if (image->depth == 1)
              {
                ssize_t
                  bit,
                  number_bits;
 
                number_bits=image->columns-x;
                if (number_bits > 8)
                  number_bits=8;
                for (bit=0; bit < number_bits; bit++)
                {
                  SetPixelIndex(indexes+x,(((unsigned char) pixel) &
                    (0x01 << (7-bit))) != 0 ? 0 : 255);
                  SetPixelRGBO(q,image->colormap+(ssize_t)
                    GetPixelIndex(indexes+x));
                  q++;
                  x++;
                }
                x--;
                continue;
              }
          }
        break;
      }
      case 1:
      {
        if (image->storage_class == PseudoClass)
          SetPixelAlpha(q,pixel);
        else
          SetPixelGreen(q,pixel);
        break;
      }
      case 2:
      {
        if (image->storage_class == PseudoClass)
          SetPixelAlpha(q,pixel);
        else
          SetPixelBlue(q,pixel);
        break;
      }
      case 3:
      {
        if (image->colorspace == CMYKColorspace)
          SetPixelIndex(indexes+x,pixel)
        else
          if (image->matte != MagickFalse)
            SetPixelAlpha(q,pixel);
        break;
      }
      case 4:
      {
        if ((IssRGBCompatibleColorspace(image->colorspace) != MagickFalse) &&
            (channels > 3))
          break;
        if (image->matte != MagickFalse)
          SetPixelAlpha(q,pixel);
        break;
      }
      default:
        break;
    }
    q++;
  }
  return(SyncAuthenticPixels(image,exception));
}
