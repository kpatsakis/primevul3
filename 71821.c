u2fh_devs_init (u2fh_devs ** devs)
{
  u2fh_devs *d;
  int rc;

  d = malloc (sizeof (*d));
  if (d == NULL)
    return U2FH_MEMORY_ERROR;

  memset (d, 0, sizeof (*d));

  rc = hid_init ();
  if (rc != 0)
    {
      free (d);
      return U2FH_TRANSPORT_ERROR;
    }

  *devs = d;

  return U2FH_OK;
}
