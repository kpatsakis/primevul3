static inline MagickSizeType GetPSDSize(const PSDInfo *psd_info,Image *image)
{
  if (psd_info->version == 1)
    return((MagickSizeType) ReadBlobMSBLong(image));
  return((MagickSizeType) ReadBlobMSBLongLong(image));
}
