set_ambient_capabilities (void)
{
  if (is_privileged)
    return;
  prctl_caps (requested_caps, FALSE, TRUE);
}
