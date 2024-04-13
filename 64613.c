ofputil_parse_key_value(char **stringp, char **keyp, char **valuep)
{
    /* Skip white space and delimiters.  If that brings us to the end of the
     * input string, we are done and there are no more key-value pairs. */
    *stringp += strspn(*stringp, ", \t\r\n");
    if (**stringp == '\0') {
        *keyp = *valuep = NULL;
        return false;
    }

    /* Extract the key and the delimiter that ends the key-value pair or begins
     * the value.  Advance the input position past the key and delimiter. */
    char *key = *stringp;
    size_t key_len = strcspn(key, ":=(, \t\r\n");
    char key_delim = key[key_len];
    key[key_len] = '\0';
    *stringp += key_len + (key_delim != '\0');

    /* Figure out what delimiter ends the value:
     *
     *     - If key_delim is ":" or "=", the value extends until white space
     *       or a comma.
     *
     *     - If key_delim is "(", the value extends until ")".
     *
     * If there is no value, we are done. */
    const char *value_delims;
    if (key_delim == ':' || key_delim == '=') {
        value_delims = ", \t\r\n";
    } else if (key_delim == '(') {
        value_delims = ")";
    } else {
        *keyp = key;
        *valuep = key + key_len; /* Empty string. */
        return true;
    }

    /* Extract the value.  Advance the input position past the value and
     * delimiter. */
    char *value = *stringp;
    size_t value_len = parse_value(value, value_delims);
    char value_delim = value[value_len];
    value[value_len] = '\0';
    *stringp += value_len + (value_delim != '\0');

    *keyp = key;
    *valuep = value;
    return true;
}
