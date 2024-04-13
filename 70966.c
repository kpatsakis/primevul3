drop_cap_bounding_set (bool drop_all)
{
  if (!drop_all)
    prctl_caps (requested_caps, TRUE, FALSE);
  else
    {
      uint32_t no_caps[2] = {0, 0};
      prctl_caps (no_caps, TRUE, FALSE);
    }
}
