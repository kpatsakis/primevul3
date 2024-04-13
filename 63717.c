is_infinite_or_zero (double x)
{
  return isnand (x) || x + x == x;
}
