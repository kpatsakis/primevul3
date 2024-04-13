static inline double DrawEpsilonReciprocal(const double x)
{
  double sign = x < 0.0 ? -1.0 : 1.0;
  return((sign*x) >= DrawEpsilon ? 1.0/x : sign*(1.0/DrawEpsilon));
}
