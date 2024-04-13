static inline const unsigned char *PushQuantumLongPixel(
  QuantumInfo *quantum_info,const unsigned char *magick_restrict pixels,
  unsigned int *quantum)
{
  register ssize_t
    i;

  register size_t
    quantum_bits;

  *quantum=0UL;
  for (i=(ssize_t) quantum_info->depth; i > 0; )
  {
    if (quantum_info->state.bits == 0)
      {
        pixels=PushLongPixel(quantum_info->endian,pixels,
          &quantum_info->state.pixel);
        quantum_info->state.bits=32U;
      }
    quantum_bits=(size_t) i;
    if (quantum_bits > quantum_info->state.bits)
      quantum_bits=quantum_info->state.bits;
    *quantum|=(((quantum_info->state.pixel >> (32U-quantum_info->state.bits)) &
      quantum_info->state.mask[quantum_bits]) << (quantum_info->depth-i));
    i-=(ssize_t) quantum_bits;
    quantum_info->state.bits-=quantum_bits;
  }
  return(pixels);
}
