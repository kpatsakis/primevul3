int readlink_malloc(const char *p, char **ret) {
        return readlinkat_malloc(AT_FDCWD, p, ret);
}
