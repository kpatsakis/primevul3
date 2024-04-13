MagickExport MagickBooleanType SetQuantumEndian(const Image *image,
  QuantumInfo *quantum_info,const EndianType endian)
{
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  assert(quantum_info != (QuantumInfo *) NULL);
  assert(quantum_info->signature == MagickSignature);
  quantum_info->endian=endian;
  return(SetQuantumDepth(image,quantum_info,quantum_info->depth));
}
