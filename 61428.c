static inline const unsigned char *PushQuantumPixel(QuantumInfo *quantum_info,
  const unsigned char *magick_restrict pixels,unsigned int *quantum)
{
  register ssize_t
    i;

  register size_t
    quantum_bits;

  *quantum=(QuantumAny) 0;
  for (i=(ssize_t) quantum_info->depth; i > 0L; )
  {
    if (quantum_info->state.bits == 0UL)
      {
        quantum_info->state.pixel=(*pixels++);
        quantum_info->state.bits=8UL;
      }
    quantum_bits=(size_t) i;
    if (quantum_bits > quantum_info->state.bits)
      quantum_bits=quantum_info->state.bits;
    i-=(ssize_t) quantum_bits;
    quantum_info->state.bits-=quantum_bits;
    *quantum=(unsigned int) ((*quantum << quantum_bits) |
      ((quantum_info->state.pixel >> quantum_info->state.bits) &~ ((~0UL) <<
      quantum_bits)));
  }
  return(pixels);
}
