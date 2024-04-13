drop_privs (bool keep_requested_caps)
{
  assert (!keep_requested_caps || !is_privileged);
  /* Drop root uid */
  if (getuid () == 0 && setuid (opt_sandbox_uid) < 0)
    die_with_error ("unable to drop root uid");

  drop_all_caps (keep_requested_caps);
}
