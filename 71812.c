get_bytes (uint8_t * rpt, size_t len, size_t num_bytes, size_t cur)
{
  /* Return if there aren't enough bytes. */
  if (cur + num_bytes >= len)
    return 0;

  if (num_bytes == 0)
    return 0;
  else if (num_bytes == 1)
    {
      return rpt[cur + 1];
    }
  else if (num_bytes == 2)
    {
      return (rpt[cur + 2] * 256 + rpt[cur + 1]);
    }
  else
    return 0;
}
