acquire_privs (void)
{
  uid_t euid, new_fsuid;

  euid = geteuid ();

  /* Are we setuid ? */
  if (real_uid != euid)
    {
      if (euid != 0)
        die ("Unexpected setuid user %d, should be 0", euid);

      is_privileged = TRUE;
      /* We want to keep running as euid=0 until at the clone()
       * operation because doing so will make the user namespace be
       * owned by root, which makes it not ptrace:able by the user as
       * it otherwise would be. After that we will run fully as the
       * user, which is necessary e.g. to be able to read from a fuse
       * mount from the user.
       *
       * However, we don't want to accidentally mis-use euid=0 for
       * escalated filesystem access before the clone(), so we set
       * fsuid to the uid.
       */
      if (setfsuid (real_uid) < 0)
        die_with_error ("Unable to set fsuid");

      /* setfsuid can't properly report errors, check that it worked (as per manpage) */
      new_fsuid = setfsuid (-1);
      if (new_fsuid != real_uid)
        die ("Unable to set fsuid (was %d)", (int)new_fsuid);

      /* We never need capabilities after execve(), so lets drop everything from the bounding set */
      drop_cap_bounding_set (TRUE);

      /* Keep only the required capabilities for setup */
      set_required_caps ();
    }
  else if (real_uid != 0 && has_caps ())
    {
      /* We have some capabilities in the non-setuid case, which should not happen.
         Probably caused by the binary being setcap instead of setuid which we
         don't support anymore */
      die ("Unexpected capabilities but not setuid, old file caps config?");
    }
  else if (real_uid == 0)
    {
      /* If our uid is 0, default to inheriting all caps; the caller
       * can drop them via --cap-drop.  This is used by at least rpm-ostree.
       * Note this needs to happen before the argument parsing of --cap-drop.
       */
      struct __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
      struct __user_cap_data_struct data[2] = { { 0 } };

      if (capget (&hdr, data) < 0)
        die_with_error ("capget (for uid == 0) failed");

      requested_caps[0] = data[0].effective;
      requested_caps[1] = data[1].effective;
    }

  /* Else, we try unprivileged user namespaces */
}
