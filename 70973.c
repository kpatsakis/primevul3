prctl_caps (uint32_t *caps, bool do_cap_bounding, bool do_set_ambient)
{
  unsigned long cap;

  /* We ignore both EINVAL and EPERM, as we are actually relying
   * on PR_SET_NO_NEW_PRIVS to ensure the right capabilities are
   * available.  EPERM in particular can happen with old, buggy
   * kernels.  See:
   *  https://github.com/projectatomic/bubblewrap/pull/175#issuecomment-278051373
   *  https://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/commit/security/commoncap.c?id=160da84dbb39443fdade7151bc63a88f8e953077
   */
  for (cap = 0; cap <= CAP_LAST_CAP; cap++)
    {
      bool keep = FALSE;
      if (cap < 32)
        {
          if (CAP_TO_MASK_0 (cap) & caps[0])
            keep = TRUE;
        }
      else
        {
          if (CAP_TO_MASK_1 (cap) & caps[1])
            keep = TRUE;
        }

      if (keep && do_set_ambient)
        {
#ifdef PR_CAP_AMBIENT
          int res = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
          if (res == -1 && !(errno == EINVAL || errno == EPERM))
            die_with_error ("Adding ambient capability %ld", cap);
#else
          /* We ignore the EINVAL that results from not having PR_CAP_AMBIENT
           * in the current kernel at runtime, so also ignore not having it
           * in the current kernel headers at compile-time */
#endif
        }

      if (!keep && do_cap_bounding)
        {
          int res = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
          if (res == -1 && !(errno == EINVAL || errno == EPERM))
            die_with_error ("Dropping capability %ld from bounds", cap);
        }
    }
}
