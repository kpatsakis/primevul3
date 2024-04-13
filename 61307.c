static inline void WriteTGAPixel(Image *image,TGAImageType image_type,
  const IndexPacket *indexes,const PixelPacket *p)
{
  if (image_type == TGAColormap || image_type == TGARLEColormap)
    (void) WriteBlobByte(image,(unsigned char) GetPixelIndex(indexes));
  else
    {
      if (image_type == TGAMonochrome || image_type == TGARLEMonochrome)
        (void) WriteBlobByte(image,ScaleQuantumToChar(ClampToQuantum(
          GetPixelLuma(image,p))));
      else
        {
          (void) WriteBlobByte(image,ScaleQuantumToChar(GetPixelBlue(p)));
          (void) WriteBlobByte(image,ScaleQuantumToChar(GetPixelGreen(p)));
          (void) WriteBlobByte(image,ScaleQuantumToChar(GetPixelRed(p)));
          if (image->matte != MagickFalse)
            (void) WriteBlobByte(image,ScaleQuantumToChar(GetPixelAlpha(p)));
        }
    }
}
