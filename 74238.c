MagickPrivate void ResetQuantumState(QuantumInfo *quantum_info)
{
  static const unsigned int mask[32] =
  {
    0x00000000U, 0x00000001U, 0x00000003U, 0x00000007U, 0x0000000fU,
    0x0000001fU, 0x0000003fU, 0x0000007fU, 0x000000ffU, 0x000001ffU,
    0x000003ffU, 0x000007ffU, 0x00000fffU, 0x00001fffU, 0x00003fffU,
    0x00007fffU, 0x0000ffffU, 0x0001ffffU, 0x0003ffffU, 0x0007ffffU,
    0x000fffffU, 0x001fffffU, 0x003fffffU, 0x007fffffU, 0x00ffffffU,
    0x01ffffffU, 0x03ffffffU, 0x07ffffffU, 0x0fffffffU, 0x1fffffffU,
    0x3fffffffU, 0x7fffffffU
  };

  assert(quantum_info != (QuantumInfo *) NULL);
  assert(quantum_info->signature == MagickSignature);
  quantum_info->state.inverse_scale=1.0;
  if (fabs(quantum_info->scale) >= MagickEpsilon)
    quantum_info->state.inverse_scale/=quantum_info->scale;
  quantum_info->state.pixel=0U;
  quantum_info->state.bits=0U;
  quantum_info->state.mask=mask;
}
