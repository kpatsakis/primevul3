static bool _parse_slab_sizes(char *s, uint32_t *slab_sizes) {
    char *b = NULL;
    uint32_t size = 0;
    int i = 0;
    uint32_t last_size = 0;

    if (strlen(s) < 1)
        return false;

    for (char *p = strtok_r(s, "-", &b);
         p != NULL;
         p = strtok_r(NULL, "-", &b)) {
        if (!safe_strtoul(p, &size) || size < settings.chunk_size
             || size > settings.slab_chunk_size_max) {
            fprintf(stderr, "slab size %u is out of valid range\n", size);
            return false;
        }
        if (last_size >= size) {
            fprintf(stderr, "slab size %u cannot be lower than or equal to a previous class size\n", size);
            return false;
        }
        if (size <= last_size + CHUNK_ALIGN_BYTES) {
            fprintf(stderr, "slab size %u must be at least %d bytes larger than previous class\n",
                    size, CHUNK_ALIGN_BYTES);
            return false;
        }
        slab_sizes[i++] = size;
        last_size = size;
        if (i >= MAX_NUMBER_OF_SLAB_CLASSES-1) {
            fprintf(stderr, "too many slab classes specified\n");
            return false;
        }
    }

    slab_sizes[i] = 0;
    return true;
}
