int chmod_and_chown(const char *path, mode_t mode, uid_t uid, gid_t gid) {
        assert(path);

        /* Under the assumption that we are running privileged we
         * first change the access mode and only then hand out
         * ownership to avoid a window where access is too open. */

        if (mode != MODE_INVALID)
                if (chmod(path, mode) < 0)
                        return -errno;

        if (uid != UID_INVALID || gid != GID_INVALID)
                if (chown(path, uid, gid) < 0)
                        return -errno;

        return 0;
}
