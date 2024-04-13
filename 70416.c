MagickExport void SetBlobExempt(Image *image,const MagickBooleanType exempt)
{
  assert(image != (const Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  image->blob->exempt=exempt;
}
