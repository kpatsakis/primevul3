int readlink_and_make_absolute_root(const char *root, const char *path, char **ret) {
        _cleanup_free_ char *target = NULL, *t = NULL;
        const char *full;
        int r;

        full = prefix_roota(root, path);
        r = readlink_malloc(full, &target);
        if (r < 0)
                return r;

        t = file_in_same_dir(path, target);
        if (!t)
                return -ENOMEM;

        *ret = t;
        t = NULL;

        return 0;
}
