njs_typed_array_prototype_iterator(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t type)
{
    double              val;
    int64_t             i, length;
    njs_int_t           ret;
    njs_arr_t           results;
    njs_value_t         *this, *this_arg, *r;
    njs_value_t         arguments[4], retval;
    njs_function_t      *function;
    njs_typed_array_t   *array, *dst;
    njs_array_buffer_t  *buffer;

    this = njs_argument(args, 0);
    if (njs_slow_path(!njs_is_typed_array(this))) {
        njs_type_error(vm, "this is not a typed array");
        return NJS_ERROR;
    }

    dst = NULL;
    array = njs_typed_array(this);
    length = njs_typed_array_length(array);

    if (njs_slow_path(!njs_is_function(njs_arg(args, nargs, 1)))) {
        njs_type_error(vm, "callback argument is not callable");
        return NJS_ERROR;
    }

    function = njs_function(njs_argument(args, 1));
    this_arg = njs_arg(args, nargs, 2);

    buffer = array->buffer;
    results.separate = 0;
    results.pointer = 0;

    switch (type) {
    case NJS_ARRAY_MAP:
        njs_set_number(&arguments[0], length);
        ret = njs_typed_array_species_create(vm, this, njs_value_arg(arguments),
                                             1, &retval);
        if (njs_slow_path(ret != NJS_OK)) {
            return NJS_ERROR;
        }

        dst = njs_typed_array(&retval);
        break;

    case NJS_ARRAY_FILTER:
    default:
        r = njs_arr_init(vm->mem_pool, &results, NULL, 4, sizeof(njs_value_t));
        if (njs_slow_path(r == NULL)) {
            return NJS_ERROR;
        }
    }

    for (i = 0; i < length; i++) {
        if (njs_slow_path(njs_is_detached_buffer(buffer))) {
            njs_type_error(vm, "detached buffer");
            return NJS_ERROR;
        }

        val = njs_typed_array_prop(array, i);

        arguments[0] = *this_arg;
        njs_set_number(&arguments[1], val);
        njs_set_number(&arguments[2], i);
        njs_set_typed_array(&arguments[3], array);

        ret = njs_function_apply(vm, function, arguments, 4, &vm->retval);
        if (njs_slow_path(ret != NJS_OK)) {
            goto exception;
        }

        switch (type) {
        case NJS_ARRAY_EVERY:
            if (!njs_is_true(&vm->retval)) {
                vm->retval = njs_value_false;
                goto done;
            }

            break;

        case NJS_ARRAY_FOR_EACH:
            break;

        case NJS_ARRAY_SOME:
        case NJS_ARRAY_FIND:
        case NJS_ARRAY_FIND_INDEX:
            if (!njs_is_true(&vm->retval)) {
                continue;
            }

            switch (type) {
            case NJS_ARRAY_SOME:
                vm->retval = njs_value_true;
                break;

            case NJS_ARRAY_FIND:
                njs_set_number(&vm->retval, val);
                break;

            default:
                njs_set_number(&vm->retval, i);
                break;
            }

            goto done;

        case NJS_ARRAY_MAP:
            ret = njs_typed_array_set_value(vm, dst, i, &vm->retval);
            if (njs_slow_path(ret != NJS_OK)) {
                goto exception;
            }

            break;

        default:
            /* NJS_ARRAY_FILTER. */

            if (!njs_is_true(&vm->retval)) {
                continue;
            }

            r = njs_arr_add(&results);
            if (njs_slow_path(r == NULL)) {
                goto exception;
            }

            njs_set_number(r, val);
        }
    }

    /* Default values. */

    switch (type) {
    case NJS_ARRAY_EVERY:
        vm->retval = njs_value_true;
        break;

    case NJS_ARRAY_SOME:
        vm->retval = njs_value_false;
        break;

    case NJS_ARRAY_FOR_EACH:
    case NJS_ARRAY_FIND:
        njs_set_undefined(&vm->retval);
        break;

    case NJS_ARRAY_FIND_INDEX:
        njs_set_number(&vm->retval, -1);
        break;

    case NJS_ARRAY_MAP:
    case NJS_ARRAY_FILTER:
    default:
        if (type == NJS_ARRAY_FILTER) {
            njs_set_number(&arguments[0], results.items);
            ret = njs_typed_array_species_create(vm, this,
                                                 njs_value_arg(arguments),
                                                 1, &retval);
            if (njs_slow_path(ret != NJS_OK)) {
                goto exception;
            }

            dst = njs_typed_array(&retval);

            i = 0;

            while (i < results.items) {
                r = njs_arr_item(&results, i);
                ret = njs_typed_array_set_value(vm, dst, i++, r);
                if (njs_slow_path(ret != NJS_OK)) {
                    goto exception;
                }
            }
        }

        njs_set_typed_array(&vm->retval, dst);
        break;
    }

done:

    ret = NJS_OK;

exception:

    njs_arr_destroy(&results);

    return ret;
}