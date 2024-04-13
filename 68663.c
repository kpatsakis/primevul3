static inline int wm_tolower(int c) {
    return ((wm_isupper(c)) ? (c | ('a' - 'A')) : c);
}
