current_getpwuid(void)
{
  uid_t ruid;

  /* GNU Hurd implementation has an extension where a process can exist in a
   * non-conforming environment, and thus be outside the realms of POSIX
   * process identifiers; on this platform, getuid() fails with a status of
   * (uid_t)(-1) and sets errno if a program is run from a non-conforming
   * environment.
   *
   * http://austingroupbugs.net/view.php?id=511
   */
  errno = 0;
  ruid = getuid ();

  return errno == 0 ? getpwuid (ruid) : NULL;
}
