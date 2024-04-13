is_infinite_or_zerol (long double x)
{
  return isnanl (x) || x + x == x;
}
