u2fh_devs_done (u2fh_devs * devs)
{
  close_devices (devs);
  hid_exit ();

  free (devs);
}
