MagickExport ssize_t WriteBlobString(Image *image,const char *string)
{
  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  assert(string != (const char *) NULL);
  return(WriteBlobStream(image,strlen(string),(const unsigned char *) string));
}
