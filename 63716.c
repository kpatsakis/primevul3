is_borderline (const char *digits, size_t precision)
{
  for (; precision > 0; precision--, digits++)
    if (*digits != '0')
      return 0;
  if (*digits != '1')
    return 0;
  digits++;
  return *digits == '\0';
}
