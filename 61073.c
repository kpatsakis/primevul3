static inline ssize_t MagickAbsoluteValue(const ssize_t x)
{
  if (x < 0)
    return(-x);
  return(x);
}
