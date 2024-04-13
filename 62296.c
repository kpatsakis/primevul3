static MagickBooleanType DecodeImage(Image *image,const size_t compression,
  unsigned char *pixels)
{
  int
    count;

  register ssize_t
    i,
    x;

  register unsigned char
    *p,
    *q;

  ssize_t
    y;

  unsigned char
    byte;

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  assert(pixels != (unsigned char *) NULL);
  (void) ResetMagickMemory(pixels,0,(size_t) image->columns*image->rows*
    sizeof(*pixels));
  byte=0;
  x=0;
  p=pixels;
  q=pixels+(size_t) image->columns*image->rows;
  for (y=0; y < (ssize_t) image->rows; )
  {
    MagickBooleanType
      status;

    if ((p < pixels) || (p > q))
      break;
    count=ReadBlobByte(image);
    if (count == EOF)
      break;
    if (count != 0)
      {
        /*
          Encoded mode.
        */
        count=(int) MagickMin((ssize_t) count,(ssize_t) (q-p));
        byte=(unsigned char) ReadBlobByte(image);
        if (compression == BI_RLE8)
          {
            for (i=0; i < (ssize_t) count; i++)
              *p++=(unsigned char) byte;
          }
        else
          {
            for (i=0; i < (ssize_t) count; i++)
              *p++=(unsigned char)
                ((i & 0x01) != 0 ? (byte & 0x0f) : ((byte >> 4) & 0x0f));
          }
        x+=count;
      }
    else
      {
        /*
          Escape mode.
        */
        count=ReadBlobByte(image);
        if (count == EOF)
          break;
        if (count == 0x01)
          return(MagickTrue);
        switch (count)
        {
          case 0x00:
          {
            /*
              End of line.
            */
            x=0;
            y++;
            p=pixels+y*image->columns;
            break;
          }
          case 0x02:
          {
            /*
              Delta mode.
            */
            x+=ReadBlobByte(image);
            y+=ReadBlobByte(image);
            p=pixels+y*image->columns+x;
            break;
          }
          default:
          {
            /*
              Absolute mode.
            */
            count=(int) MagickMin((ssize_t) count,(ssize_t) (q-p));
            if (compression == BI_RLE8)
              for (i=0; i < (ssize_t) count; i++)
                *p++=(unsigned char) ReadBlobByte(image);
            else
              for (i=0; i < (ssize_t) count; i++)
              {
                if ((i & 0x01) == 0)
                  byte=(unsigned char) ReadBlobByte(image);
                *p++=(unsigned char)
                  ((i & 0x01) != 0 ? (byte & 0x0f) : ((byte >> 4) & 0x0f));
              }
            x+=count;
            /*
              Read pad byte.
            */
            if (compression == BI_RLE8)
              {
                if ((count & 0x01) != 0)
                  (void) ReadBlobByte(image);
              }
            else
              if (((count & 0x03) == 1) || ((count & 0x03) == 2))
                (void) ReadBlobByte(image);
            break;
          }
        }
      }
    status=SetImageProgress(image,LoadImageTag,(MagickOffsetType) y,
      image->rows);
    if (status == MagickFalse)
      break;
  }
  (void) ReadBlobByte(image);  /* end of line */
  (void) ReadBlobByte(image);
  return(y < (ssize_t) image->rows ? MagickFalse : MagickTrue);
}
