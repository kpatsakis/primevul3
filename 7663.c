njs_typed_array_of(njs_vm_t *vm, njs_value_t *args, njs_uint_t nargs,
    njs_index_t unused)
{
    double             num;
    uint32_t           length, i;
    njs_int_t          ret;
    njs_value_t        *this;
    njs_value_t        argument;
    njs_typed_array_t  *array;

    this = njs_argument(args, 0);

    if (njs_slow_path(!njs_is_constructor(this))) {
        njs_type_error(vm, "%s is not a constructor",
                       njs_type_string(this->type));
        return NJS_ERROR;
    }

    length = nargs - 1;

    njs_set_number(&argument, length);
    ret = njs_typed_array_create(vm, this, &argument, 1, &vm->retval);
    if (njs_slow_path(ret != NJS_OK)) {
        return NJS_ERROR;
    }

    array = njs_typed_array(&vm->retval);

    for (i = 0; i < length; i++) {
        ret = njs_value_to_number(vm, njs_argument(args, i + 1), &num);
        if (njs_slow_path(ret != NJS_OK)) {
            return NJS_ERROR;
        }

        njs_typed_array_prop_set(vm, array, i, num);
    }

    njs_set_typed_array(&vm->retval, array);

    return NJS_OK;
}