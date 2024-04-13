MagickExport unsigned char *GetQuantumPixels(const QuantumInfo *quantum_info)
{
  const int
    id = GetOpenMPThreadId();

  assert(quantum_info != (QuantumInfo *) NULL);
  assert(quantum_info->signature == MagickSignature);
  return(quantum_info->pixels[id]);
}
