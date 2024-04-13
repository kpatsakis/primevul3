static ssize_t DecodePSDPixels(const size_t number_compact_pixels,
  const unsigned char *compact_pixels,const ssize_t depth,
  const size_t number_pixels,unsigned char *pixels)
{
  int
    pixel;

  register ssize_t
    i,
    j;

  size_t
    length;

  ssize_t
    packets;

  packets=(ssize_t) number_compact_pixels;
  for (i=0; (packets > 1) && (i < (ssize_t) number_pixels); )
  {
    length=(size_t) (*compact_pixels++);
    packets--;
    if (length == 128)
      continue;
    if (length > 128)
      {
        length=256-length+1;
        if (((ssize_t) length+i) > (ssize_t) number_pixels)
          length=number_pixels-(size_t) i;
        pixel=(*compact_pixels++);
        packets--;
        for (j=0; j < (ssize_t) length; j++)
        {
          switch (depth)
          {
            case 1:
            {
              *pixels++=(pixel >> 7) & 0x01 ? 0U : 255U;
              *pixels++=(pixel >> 6) & 0x01 ? 0U : 255U;
              *pixels++=(pixel >> 5) & 0x01 ? 0U : 255U;
              *pixels++=(pixel >> 4) & 0x01 ? 0U : 255U;
              *pixels++=(pixel >> 3) & 0x01 ? 0U : 255U;
              *pixels++=(pixel >> 2) & 0x01 ? 0U : 255U;
              *pixels++=(pixel >> 1) & 0x01 ? 0U : 255U;
              *pixels++=(pixel >> 0) & 0x01 ? 0U : 255U;
              i+=8;
              break;
            }
            case 4:
            {
              *pixels++=(unsigned char) ((pixel >> 4) & 0xff);
              *pixels++=(unsigned char) ((pixel & 0x0f) & 0xff);
              i+=2;
              break;
            }
            case 2:
            {
              *pixels++=(unsigned char) ((pixel >> 6) & 0x03);
              *pixels++=(unsigned char) ((pixel >> 4) & 0x03);
              *pixels++=(unsigned char) ((pixel >> 2) & 0x03);
              *pixels++=(unsigned char) ((pixel & 0x03) & 0x03);
              i+=4;
              break;
            }
            default:
            {
              *pixels++=(unsigned char) pixel;
              i++;
              break;
            }
          }
        }
        continue;
      }
    length++;
    if (((ssize_t) length+i) > (ssize_t) number_pixels)
      length=number_pixels-(size_t) i;
    for (j=0; j < (ssize_t) length; j++)
    {
      switch (depth)
      {
        case 1:
        {
          *pixels++=(*compact_pixels >> 7) & 0x01 ? 0U : 255U;
          *pixels++=(*compact_pixels >> 6) & 0x01 ? 0U : 255U;
          *pixels++=(*compact_pixels >> 5) & 0x01 ? 0U : 255U;
          *pixels++=(*compact_pixels >> 4) & 0x01 ? 0U : 255U;
          *pixels++=(*compact_pixels >> 3) & 0x01 ? 0U : 255U;
          *pixels++=(*compact_pixels >> 2) & 0x01 ? 0U : 255U;
          *pixels++=(*compact_pixels >> 1) & 0x01 ? 0U : 255U;
          *pixels++=(*compact_pixels >> 0) & 0x01 ? 0U : 255U;
          i+=8;
          break;
        }
        case 4:
        {
          *pixels++=(*compact_pixels >> 4) & 0xff;
          *pixels++=(*compact_pixels & 0x0f) & 0xff;
          i+=2;
          break;
        }
        case 2:
        {
          *pixels++=(*compact_pixels >> 6) & 0x03;
          *pixels++=(*compact_pixels >> 4) & 0x03;
          *pixels++=(*compact_pixels >> 2) & 0x03;
          *pixels++=(*compact_pixels & 0x03) & 0x03;
          i+=4;
          break;
        }
        default:
        {
          *pixels++=(*compact_pixels);
          i++;
          break;
        }
      }
      compact_pixels++;
    }
  }
  return(i);
}
