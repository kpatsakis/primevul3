MagickExport QuantumType GetQuantumType(Image *image,ExceptionInfo *exception)
{
  QuantumType
    quantum_type;

  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  (void) exception;
  quantum_type=RGBQuantum;
  if (image->matte != MagickFalse)
    quantum_type=RGBAQuantum;
  if (image->colorspace == CMYKColorspace)
    {
      quantum_type=CMYKQuantum;
      if (image->matte != MagickFalse)
        quantum_type=CMYKAQuantum;
    }
  if (IsGrayColorspace(image->colorspace) != MagickFalse)
    {
      quantum_type=GrayQuantum;
      if (image->matte != MagickFalse)
        quantum_type=GrayAlphaQuantum;
    }
  if (image->storage_class == PseudoClass)
    {
      quantum_type=IndexQuantum;
      if (image->matte != MagickFalse)
        quantum_type=IndexAlphaQuantum;
    }
  return(quantum_type);
}
