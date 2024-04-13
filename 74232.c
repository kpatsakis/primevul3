static void DestroyQuantumPixels(QuantumInfo *quantum_info)
{
  register ssize_t
    i;

  ssize_t
    extent;

  assert(quantum_info != (QuantumInfo *) NULL);
  assert(quantum_info->signature == MagickSignature);
  assert(quantum_info->pixels != (unsigned char **) NULL);
  extent=(ssize_t) quantum_info->extent;
  for (i=0; i < (ssize_t) quantum_info->number_threads; i++)
    if (quantum_info->pixels[i] != (unsigned char *) NULL)
      {
        /*
          Did we overrun our quantum buffer?
        */
        assert(quantum_info->pixels[i][extent] == QuantumSignature);
        quantum_info->pixels[i]=(unsigned char *) RelinquishMagickMemory(
          quantum_info->pixels[i]);
      }
  quantum_info->pixels=(unsigned char **) RelinquishMagickMemory(
    quantum_info->pixels);
}
