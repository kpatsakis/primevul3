static FP32 half_to_float(FP16 h) {
  static const FP32 magic = {113 << 23};
  static const unsigned int shifted_exp = 0x7c00
                                          << 13;  // exponent mask after shift
  FP32 o;

  o.u = (h.u & 0x7fffU) << 13U;           // exponent/mantissa bits
  unsigned int exp_ = shifted_exp & o.u;  // just the exponent
  o.u += (127 - 15) << 23;                // exponent adjust

  // handle exponent special cases
  if (exp_ == shifted_exp)    // Inf/NaN?
    o.u += (128 - 16) << 23;  // extra exp adjust
  else if (exp_ == 0)         // Zero/Denormal?
  {
    o.u += 1 << 23;  // extra exp adjust
    o.f -= magic.f;  // renormalize
  }

  o.u |= (h.u & 0x8000U) << 16U;  // sign bit
  return o;
}