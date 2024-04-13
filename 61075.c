static inline ssize_t MagickMin(const ssize_t x,const ssize_t y)
{
  if (x < y)
    return(x);
  return(y);
}
