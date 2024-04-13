static inline void cp_html_entity (char *buffer, const char *entity) {
    if (!buffer || !entity)
        return;
    while (*entity)
        *buffer++ = *entity++;
}
