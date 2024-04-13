njs_typed_array_prototype_copy_within(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t unused)
{
    int64_t             length, to, from, final, count;
    uint32_t            element_size;
    njs_int_t           ret;
    njs_value_t         *this, *value;
    njs_typed_array_t   *array;
    njs_array_buffer_t  *buffer;

    this = njs_argument(args, 0);
    if (njs_slow_path(!njs_is_typed_array(this))) {
        njs_type_error(vm, "this is not a typed array");
        return NJS_ERROR;
    }

    array = njs_typed_array(this);
    if (njs_slow_path(njs_is_detached_buffer(array->buffer))) {
        njs_type_error(vm, "detached buffer");
        return NJS_ERROR;
    }

    length = njs_typed_array_length(array);

    value = njs_arg(args, nargs, 1);

    ret = njs_value_to_integer(vm, value, &to);
    if (njs_slow_path(ret != NJS_OK)) {
        return NJS_ERROR;
    }

    to = to < 0 ? njs_max(to + length, 0) : njs_min(to, length);

    value = njs_arg(args, nargs, 2);

    ret = njs_value_to_integer(vm, value, &from);
    if (njs_slow_path(ret != NJS_OK)) {
        return ret;
    }

    from = from < 0 ? njs_max(from + length, 0) : njs_min(from, length);

    value = njs_arg(args, nargs, 3);

    final = length;

    if (njs_is_defined(value)) {
        ret = njs_value_to_integer(vm, value, &final);
        if (njs_slow_path(ret != NJS_OK)) {
            return NJS_ERROR;
        }
    }

    final = (final < 0) ? njs_max(final + length, 0) : njs_min(final, length);

    njs_set_typed_array(&vm->retval, array);

    count = njs_min(final - from, length - to);

    if (count <= 0) {
        return NJS_OK;
    }

    buffer = njs_typed_array_writable(vm, array);
    if (njs_slow_path(buffer == NULL)) {
        return NJS_ERROR;
    }

    element_size = njs_typed_array_element_size(array->type);

    to = (to + array->offset) * element_size;
    from = (from + array->offset) * element_size;
    count = count * element_size;

    memmove(&buffer->u.u8[to], &buffer->u.u8[from], count);

    return NJS_OK;
}