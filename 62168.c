inline static void copy_line(int16_t *buf, const int16_t *ptr, uintptr_t offs, uintptr_t size)
{
    ptr = get_line(ptr, offs, size);
    for (int k = 0; k < STRIPE_WIDTH; ++k)
        buf[k] = ptr[k];
}
