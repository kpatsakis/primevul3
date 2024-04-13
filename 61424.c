static inline Quantum PushColormapIndex(const Image *image,const size_t index,
  MagickBooleanType *range_exception)
{
  if (index < image->colors)
    return((Quantum) index);
  *range_exception=MagickTrue;
  return((Quantum) 0);
}
