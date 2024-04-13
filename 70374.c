MagickExport void DuplicateBlob(Image *image,const Image *duplicate)
{
  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  assert(duplicate != (Image *) NULL);
  assert(duplicate->signature == MagickCoreSignature);
  DestroyBlob(image);
  image->blob=ReferenceBlob(duplicate->blob);
}
