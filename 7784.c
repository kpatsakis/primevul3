next_segment(svn_stringbuf_t *segment,
             const char *path)
{
  apr_size_t len;
  char c;

  /* Read and scan PATH for NUL and '/' -- whichever comes first. */
  for (len = 0, c = *path; c; c = path[++len])
    if (c == '/')
      {
        /* End of segment. */
        segment->data[len] = 0;
        segment->len = len;

        /* If PATH is not normalized, this is where we skip whole sequences
         * of separators. */
        while (path[++len] == '/')
          ;

        /* Continue behind the last separator in the sequence.  We will
         * treat trailing '/' as indicating an empty trailing segment.
         * Therefore, we never have to return NULL here. */
        return path + len;
      }
    else
      {
        /* Copy segment contents directly into the result buffer.
         * On many architectures, this is almost or entirely for free. */
        segment->data[len] = c;
      }

  /* No separator found, so all of PATH has been the last segment. */
  segment->data[len] = 0;
  segment->len = len;

  /* Tell the caller that this has been the last segment. */
  return NULL;
}