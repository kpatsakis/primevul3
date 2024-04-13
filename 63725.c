scale10_round_decimal_long_double (long double x, int n)
{
  int e IF_LINT(= 0);
  mpn_t m;
  void *memory = decode_long_double (x, &e, &m);
  return scale10_round_decimal_decoded (e, m, memory, n);
}
