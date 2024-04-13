int rmdir_parents(const char *path, const char *stop) {
        size_t l;
        int r = 0;

        assert(path);
        assert(stop);

        l = strlen(path);

        /* Skip trailing slashes */
        while (l > 0 && path[l-1] == '/')
                l--;

        while (l > 0) {
                char *t;

                /* Skip last component */
                while (l > 0 && path[l-1] != '/')
                        l--;

                /* Skip trailing slashes */
                while (l > 0 && path[l-1] == '/')
                        l--;

                if (l <= 0)
                        break;

                t = strndup(path, l);
                if (!t)
                        return -ENOMEM;

                if (path_startswith(stop, t)) {
                        free(t);
                        return 0;
                }

                r = rmdir(t);
                free(t);

                if (r < 0)
                        if (errno != ENOENT)
                                return -errno;
        }

        return 0;
}
