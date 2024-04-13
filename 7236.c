static void *def_alloc_func(void *opaque, size_t items, size_t size) {
  (void)opaque, (void)items, (void)size;
  return MZ_MALLOC(items * size);
}