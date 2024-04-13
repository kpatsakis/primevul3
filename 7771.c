njs_module_read(njs_vm_t *vm, int fd, njs_str_t *text)
{
    ssize_t      n;
    struct stat  sb;

    text->start = NULL;

    if (fstat(fd, &sb) == -1) {
        goto fail;
    }

    if (!S_ISREG(sb.st_mode)) {
        goto fail;
    }

    text->length = sb.st_size;

    text->start = njs_mp_alloc(vm->mem_pool, text->length);
    if (text->start == NULL) {
        goto fail;
    }

    n = read(fd, text->start, sb.st_size);

    if (n < 0 || n != sb.st_size) {
        goto fail;
    }

    return NJS_OK;

fail:

    if (text->start != NULL) {
        njs_mp_free(vm->mem_pool, text->start);
    }

    return NJS_ERROR;
}