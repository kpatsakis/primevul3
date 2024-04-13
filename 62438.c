MagickExport const char *GetMagickQuantumDepth(size_t *depth)
{
  if (depth != (size_t *) NULL)
    *depth=(size_t) MAGICKCORE_QUANTUM_DEPTH;
  return(MagickQuantumDepth);
}
