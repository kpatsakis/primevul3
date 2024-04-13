njs_typed_array_prototype_reduce(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t right)
{
    int64_t             i, from, to, increment, length;
    njs_int_t           ret;
    njs_value_t         *this, accumulator;
    njs_value_t         arguments[5];
    njs_function_t      *function;
    njs_typed_array_t   *array;
    njs_array_buffer_t  *buffer;

    this = njs_argument(args, 0);
    if (njs_slow_path(!njs_is_typed_array(this))) {
        njs_type_error(vm, "this is not a typed array");
        return NJS_ERROR;
    }

    array = njs_typed_array(this);
    length = njs_typed_array_length(array);

    if (njs_slow_path(!njs_is_function(njs_arg(args, nargs, 1)))) {
        njs_type_error(vm, "callback argument is not callable");
        return NJS_ERROR;
    }

    function = njs_function(njs_argument(args, 1));

    if (length == 0 && nargs <= 2) {
        njs_type_error(vm, "Reduce of empty object with no initial value");
        return NJS_ERROR;
    }

    if (right) {
        from = length - 1;
        to = -1;
        increment = -1;

    } else {
        from = 0;
        to = length;
        increment = 1;
    }

    buffer = array->buffer;

    if (nargs > 2) {
        accumulator = *njs_argument(args, 2);

    } else {
        if (njs_slow_path(njs_is_detached_buffer(buffer))) {
            njs_type_error(vm, "detached buffer");
            return NJS_ERROR;
        }

        njs_set_number(&accumulator, njs_typed_array_prop(array, from));
        from += increment;
    }

    for (i = from; i != to; i += increment) {
        if (njs_slow_path(njs_is_detached_buffer(buffer))) {
            njs_type_error(vm, "detached buffer");
            return NJS_ERROR;
        }

        njs_set_undefined(&arguments[0]);
        arguments[1] = accumulator;
        njs_set_number(&arguments[2], njs_typed_array_prop(array, i));
        njs_set_number(&arguments[3], i);
        njs_set_typed_array(&arguments[4], array);

        ret =  njs_function_apply(vm, function, arguments, 5, &accumulator);
        if (njs_slow_path(ret != NJS_OK)) {
            return ret;
        }
    }

    vm->retval = accumulator;

    return NJS_OK;
}