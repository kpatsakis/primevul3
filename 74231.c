MagickExport QuantumInfo *DestroyQuantumInfo(QuantumInfo *quantum_info)
{
  assert(quantum_info != (QuantumInfo *) NULL);
  assert(quantum_info->signature == MagickSignature);
  if (quantum_info->pixels != (unsigned char **) NULL)
    DestroyQuantumPixels(quantum_info);
  if (quantum_info->semaphore != (SemaphoreInfo *) NULL)
    DestroySemaphoreInfo(&quantum_info->semaphore);
  quantum_info->signature=(~MagickSignature);
  quantum_info=(QuantumInfo *) RelinquishMagickMemory(quantum_info);
  return(quantum_info);
}
