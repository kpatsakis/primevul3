scale10_round_decimal_double (double x, int n)
{
  int e IF_LINT(= 0);
  mpn_t m;
  void *memory = decode_double (x, &e, &m);
  return scale10_round_decimal_decoded (e, m, memory, n);
}
