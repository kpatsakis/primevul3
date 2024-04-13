int rename_noreplace(int olddirfd, const char *oldpath, int newdirfd, const char *newpath) {
        struct stat buf;
        int ret;

        ret = renameat2(olddirfd, oldpath, newdirfd, newpath, RENAME_NOREPLACE);
        if (ret >= 0)
                return 0;

        /* renameat2() exists since Linux 3.15, btrfs added support for it later.
         * If it is not implemented, fallback to another method. */
        if (!IN_SET(errno, EINVAL, ENOSYS))
                return -errno;

        /* The link()/unlink() fallback does not work on directories. But
         * renameat() without RENAME_NOREPLACE gives the same semantics on
         * directories, except when newpath is an *empty* directory. This is
         * good enough. */
        ret = fstatat(olddirfd, oldpath, &buf, AT_SYMLINK_NOFOLLOW);
        if (ret >= 0 && S_ISDIR(buf.st_mode)) {
                ret = renameat(olddirfd, oldpath, newdirfd, newpath);
                return ret >= 0 ? 0 : -errno;
        }

        /* If it is not a directory, use the link()/unlink() fallback. */
        ret = linkat(olddirfd, oldpath, newdirfd, newpath, 0);
        if (ret < 0)
                return -errno;

        ret = unlinkat(olddirfd, oldpath, 0);
        if (ret < 0) {
                /* backup errno before the following unlinkat() alters it */
                ret = errno;
                (void) unlinkat(newdirfd, newpath, 0);
                errno = ret;
                return -errno;
        }

        return 0;
}
