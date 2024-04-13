static MagickBooleanType AcquireQuantumPixels(QuantumInfo *quantum_info,
  const size_t extent)
{
  register ssize_t
    i;

  assert(quantum_info != (QuantumInfo *) NULL);
  assert(quantum_info->signature == MagickSignature);
  quantum_info->number_threads=(size_t) GetMagickResourceLimit(ThreadResource);
  quantum_info->pixels=(unsigned char **) AcquireQuantumMemory(
    quantum_info->number_threads,sizeof(*quantum_info->pixels));
  if (quantum_info->pixels == (unsigned char **) NULL)
    return(MagickFalse);
  quantum_info->extent=extent;
  (void) ResetMagickMemory(quantum_info->pixels,0,quantum_info->number_threads*
    sizeof(*quantum_info->pixels));
  for (i=0; i < (ssize_t) quantum_info->number_threads; i++)
  {
    quantum_info->pixels[i]=(unsigned char *) AcquireQuantumMemory(extent+1,
      sizeof(**quantum_info->pixels));
    if (quantum_info->pixels[i] == (unsigned char *) NULL)
      {
        while (--i >= 0)
          quantum_info->pixels[i]=(unsigned char *) RelinquishMagickMemory(
            quantum_info->pixels[i]);
        return(MagickFalse);
      }
    (void) ResetMagickMemory(quantum_info->pixels[i],0,(extent+1)*
      sizeof(**quantum_info->pixels));
    quantum_info->pixels[i][extent]=QuantumSignature;
  }
  return(MagickTrue);
}
