static void def_free_func(void *opaque, void *address) {
  (void)opaque, (void)address;
  MZ_FREE(address);
}