njs_typed_array_prototype_join(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t unused)
{
    u_char             *p;
    size_t             size, length, arr_length;
    njs_int_t          ret;
    njs_chb_t          chain;
    njs_value_t        *this, *separator;
    njs_typed_array_t  *array;

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

    arr_length = njs_typed_array_length(array);

    separator = njs_arg(args, nargs, 1);

    if (njs_slow_path(!njs_is_string(separator))) {
        if (njs_is_undefined(separator)) {
            separator = njs_value_arg(&njs_string_comma);

        } else {
            ret = njs_value_to_string(vm, separator, separator);
            if (njs_slow_path(ret != NJS_OK)) {
                return ret;
            }
        }
    }

    if (arr_length == 0) {
        vm->retval = njs_string_empty;
        return NJS_OK;
    }

    if (njs_slow_path(njs_is_detached_buffer(array->buffer))) {
        njs_type_error(vm, "detached buffer");
        return NJS_ERROR;
    }

    njs_chb_init(&chain, vm->mem_pool);

    length = njs_typed_array_to_chain(vm, &chain, array, separator);
    size = njs_chb_size(&chain);

    p = njs_string_alloc(vm, &vm->retval, size, length);
    if (njs_slow_path(p == NULL)) {
        return NJS_ERROR;
    }

    njs_chb_join_to(&chain, p);
    njs_chb_destroy(&chain);

    return NJS_OK;
}