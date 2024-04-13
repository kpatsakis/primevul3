MagickExport MagickBooleanType SetQuantumPad(const Image *image,
  QuantumInfo *quantum_info,const size_t pad)
{
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  assert(quantum_info != (QuantumInfo *) NULL);
  assert(quantum_info->signature == MagickSignature);
  quantum_info->pad=pad;
  return(SetQuantumDepth(image,quantum_info,quantum_info->depth));
}
