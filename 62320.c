static void rwpng_free_chunks(struct rwpng_chunk *chunk) {
    if (!chunk) return;
    rwpng_free_chunks(chunk->next);
    free(chunk->data);
    free(chunk);
}
