MagickExport QuantumInfo *AcquireQuantumInfo(const ImageInfo *image_info,
  Image *image)
{
  MagickBooleanType
    status;

  QuantumInfo
    *quantum_info;

  quantum_info=(QuantumInfo *) AcquireMagickMemory(sizeof(*quantum_info));
  if (quantum_info == (QuantumInfo *) NULL)
    ThrowFatalException(ResourceLimitFatalError,"MemoryAllocationFailed");
  quantum_info->signature=MagickSignature;
  GetQuantumInfo(image_info,quantum_info);
  if (image == (const Image *) NULL)
    return(quantum_info);
  status=SetQuantumDepth(image,quantum_info,image->depth);
  quantum_info->endian=image->endian;
  if (status == MagickFalse)
    quantum_info=DestroyQuantumInfo(quantum_info);
  return(quantum_info);
}
