static inline MagickBooleanType IsPoint(const char *point)
{
  char
    *p;

  double
    value;

  value=StringToDouble(point,&p);
  return((fabs(value) < DrawEpsilon) && (p == point) ? MagickFalse : MagickTrue);
}
