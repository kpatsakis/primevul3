drop_all_caps (bool keep_requested_caps)
{
  struct __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  struct __user_cap_data_struct data[2] = { { 0 } };

  if (keep_requested_caps)
    {
      /* Avoid calling capset() unless we need to; currently
       * systemd-nspawn at least is known to install a seccomp
       * policy denying capset() for dubious reasons.
       * <https://github.com/projectatomic/bubblewrap/pull/122>
       */
      if (!opt_cap_add_or_drop_used && real_uid == 0)
        {
          assert (!is_privileged);
          return;
        }
      data[0].effective = requested_caps[0];
      data[0].permitted = requested_caps[0];
      data[0].inheritable = requested_caps[0];
      data[1].effective = requested_caps[1];
      data[1].permitted = requested_caps[1];
      data[1].inheritable = requested_caps[1];
    }

  if (capset (&hdr, data) < 0)
    {
      /* While the above logic ensures we don't call capset() for the primary
       * process unless configured to do so, we still try to drop privileges for
       * the init process unconditionally. Since due to the systemd seccomp
       * filter that will fail, let's just ignore it.
       */
      if (errno == EPERM && real_uid == 0 && !is_privileged)
        return;
      else
        die_with_error ("capset failed");
    }
}
