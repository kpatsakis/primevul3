MagickExport FILE *GetBlobFileHandle(const Image *image)
{
  assert(image != (const Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  return(image->blob->file_info.file);
}
