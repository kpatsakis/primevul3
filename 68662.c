static char *wm_strdup (const char *str) {
    size_t l = strlen(str) + 5;
    char *d = (char *) malloc(l * sizeof(char));
    if (d) {
        strcpy(d, str);
        return (d);
    }
    return (NULL);
}
