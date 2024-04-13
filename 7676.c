njs_typed_array_writable(njs_vm_t *vm, njs_typed_array_t *array)
{
    njs_int_t           ret;
    njs_array_buffer_t  *buffer;

    buffer = array->buffer;
    if (njs_slow_path(njs_is_detached_buffer(buffer))) {
        njs_type_error(vm, "detached buffer");
        return NULL;
    }

    ret = njs_array_buffer_writable(vm, buffer);
    if (njs_slow_path(ret != NJS_OK)) {
        return NULL;
    }

    return buffer;
}