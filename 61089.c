static void InsertRow(ssize_t depth,unsigned char *p,ssize_t y,Image *image)
{
  ExceptionInfo
    *exception;

  size_t bit; ssize_t x;
  register PixelPacket *q;
  IndexPacket index;
  register IndexPacket *indexes;

  index=(IndexPacket) 0;
  exception=(&image->exception);
  switch (depth)
  {
    case 1:  /* Convert bitmap scanline. */
      {
        q=QueueAuthenticPixels(image,0,y,image->columns,1,exception);
        if (q == (PixelPacket *) NULL)
          break;
        indexes=GetAuthenticIndexQueue(image);
        for (x=0; x < ((ssize_t) image->columns-7); x+=8)
        {
          for (bit=0; bit < 8; bit++)
          {
            index=(IndexPacket) ((((*p) & (0x80 >> bit)) != 0) ? 0x01 : 0x00);
            SetPixelIndex(indexes+x+bit,index);
          }
          p++;
        }
        if ((image->columns % 8) != 0)
          {
            for (bit=0; bit < (image->columns % 8); bit++)
              {
                index=(IndexPacket) ((((*p) & (0x80 >> bit)) != 0) ? 0x01 : 0x00);
                SetPixelIndex(indexes+x+bit,index);
              }
            p++;
          }
        if (SyncAuthenticPixels(image,exception) == MagickFalse)
          break;
        break;
      }
    case 2:  /* Convert PseudoColor scanline. */
      {
        q=QueueAuthenticPixels(image,0,y,image->columns,1,exception);
        if (q == (PixelPacket *) NULL)
          break;
        indexes=GetAuthenticIndexQueue(image);
        for (x=0; x < ((ssize_t) image->columns-1); x+=2)
          {
            index=ConstrainColormapIndex(image,(*p >> 6) & 0x3);
            SetPixelIndex(indexes+x,index);
            index=ConstrainColormapIndex(image,(*p >> 4) & 0x3);
            SetPixelIndex(indexes+x,index);
            index=ConstrainColormapIndex(image,(*p >> 2) & 0x3);
            SetPixelIndex(indexes+x,index);
            index=ConstrainColormapIndex(image,(*p) & 0x3);
            SetPixelIndex(indexes+x+1,index);
            p++;
          }
        if ((image->columns % 4) != 0)
          {
            index=ConstrainColormapIndex(image,(*p >> 6) & 0x3);
            SetPixelIndex(indexes+x,index);
            if ((image->columns % 4) >= 1)

              {
                index=ConstrainColormapIndex(image,(*p >> 4) & 0x3);
                SetPixelIndex(indexes+x,index);
                if ((image->columns % 4) >= 2)

                  {
                    index=ConstrainColormapIndex(image,(*p >> 2) & 0x3);
                    SetPixelIndex(indexes+x,index);
                  }
              }
            p++;
          }
        if (SyncAuthenticPixels(image,exception) == MagickFalse)
          break;
        break;
      }

    case 4:  /* Convert PseudoColor scanline. */
      {
        q=QueueAuthenticPixels(image,0,y,image->columns,1,exception);
        if (q == (PixelPacket *) NULL)
          break;
        indexes=GetAuthenticIndexQueue(image);
        for (x=0; x < ((ssize_t) image->columns-1); x+=2)
          {
            index=ConstrainColormapIndex(image,(*p >> 4) & 0xf);
            SetPixelIndex(indexes+x,index);
            index=ConstrainColormapIndex(image,(*p) & 0xf);
            SetPixelIndex(indexes+x+1,index);
            p++;
          }
        if ((image->columns % 2) != 0)
          {
            index=ConstrainColormapIndex(image,(*p >> 4) & 0xf);
            SetPixelIndex(indexes+x,index);
            p++;
          }
        if (SyncAuthenticPixels(image,exception) == MagickFalse)
          break;
        break;
      }
    case 8: /* Convert PseudoColor scanline. */
      {
        q=QueueAuthenticPixels(image,0,y,image->columns,1,exception);
        if (q == (PixelPacket *) NULL) break;
        indexes=GetAuthenticIndexQueue(image);
        for (x=0; x < (ssize_t) image->columns; x++)
        {
          index=ConstrainColormapIndex(image,*p);
          SetPixelIndex(indexes+x,index);
          p++;
        }
        if (SyncAuthenticPixels(image,exception) == MagickFalse)
          break;
      }
      break;

    }
}
