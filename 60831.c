int symlink_idempotent(const char *from, const char *to) {
        _cleanup_free_ char *p = NULL;
        int r;

        assert(from);
        assert(to);

        if (symlink(from, to) < 0) {
                if (errno != EEXIST)
                        return -errno;

                r = readlink_malloc(to, &p);
                if (r < 0)
                        return r;

                if (!streq(p, from))
                        return -EINVAL;
        }

        return 0;
}
