static void WritePackbitsLength(const PSDInfo *psd_info,
  const ImageInfo *image_info,Image *image,Image *next_image,
  unsigned char *compact_pixels,const QuantumType quantum_type)
{
  QuantumInfo
    *quantum_info;

  register const PixelPacket
    *p;

  size_t
    length,
    packet_size;

  ssize_t
    y;

  unsigned char
    *pixels;

  if (next_image->depth > 8)
    next_image->depth=16;
  packet_size=next_image->depth > 8UL ? 2UL : 1UL;
  (void) packet_size;
  quantum_info=AcquireQuantumInfo(image_info,image);
  pixels=GetQuantumPixels(quantum_info);
  for (y=0; y < (ssize_t) next_image->rows; y++)
  {
    p=GetVirtualPixels(next_image,0,y,next_image->columns,1,&image->exception);
    if (p == (const PixelPacket *) NULL)
      break;
    length=ExportQuantumPixels(next_image,(CacheView *) NULL,quantum_info,
      quantum_type,pixels,&image->exception);
    length=PSDPackbitsEncodeImage(image,length,pixels,compact_pixels);
    (void) SetPSDOffset(psd_info,image,length);
  }
  quantum_info=DestroyQuantumInfo(quantum_info);
}
