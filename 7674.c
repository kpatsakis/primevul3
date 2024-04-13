njs_typed_array_from(njs_vm_t *vm, njs_value_t *args, njs_uint_t nargs,
    njs_index_t unused)
{
    double             num;
    int64_t            length, i;
    njs_int_t          ret;
    njs_value_t        *this, *source, *mapfn;
    njs_value_t        arguments[3], retval;
    njs_function_t     *function;
    njs_typed_array_t  *array;

    this = njs_argument(args, 0);

    if (njs_slow_path(!njs_is_constructor(this))) {
        njs_type_error(vm, "%s is not a constructor",
                       njs_type_string(this->type));
        return NJS_ERROR;
    }

    mapfn = njs_arg(args, nargs, 2);

    if (njs_slow_path(!njs_is_function_or_undefined(mapfn))) {
        njs_type_error(vm, "\"mapfn\" argument is not callable");
        return NJS_ERROR;
    }

    function = NULL;
    if (njs_is_function(mapfn)) {
        function = njs_function(mapfn);
    }

    source = njs_arg(args, nargs, 1);

    ret = njs_value_to_object(vm, source);
    if (njs_slow_path(ret != NJS_OK)) {
        return ret;
    }

    ret = njs_object_length(vm, source, &length);
    if (njs_slow_path(ret == NJS_ERROR)) {
        return ret;
    }

    njs_set_number(&arguments[0], length);
    ret = njs_typed_array_create(vm, this, arguments, 1, &vm->retval);
    if (njs_slow_path(ret != NJS_OK)) {
        return NJS_ERROR;
    }

    array = njs_typed_array(&vm->retval);
    arguments[0] = *njs_arg(args, nargs, 3);

    for (i = 0; i < length; i++) {
        ret = njs_value_property_i64(vm, source, i, &retval);
        if (njs_slow_path(ret == NJS_ERROR)) {
            return NJS_ERROR;
        }

        if (function != NULL) {
            arguments[1] = retval;
            njs_set_number(&arguments[2], i);
            ret = njs_function_apply(vm, function, arguments, 3, &retval);
            if (njs_slow_path(ret != NJS_OK)) {
                return NJS_ERROR;
            }
        }

        ret = njs_value_to_number(vm, &retval, &num);
        if (njs_slow_path(ret != NJS_OK)) {
            return NJS_ERROR;
        }

        njs_typed_array_prop_set(vm, array, i, num);
    }

    njs_set_typed_array(&vm->retval, array);

    return NJS_OK;
}