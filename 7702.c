njs_typed_array_prototype_slice(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t copy)
{
    int64_t             start, end, count, offset;
    uint32_t            i, element_size, length;
    njs_int_t           ret;
    njs_value_t         arguments[3], *this, *value;
    njs_typed_array_t   *array, *new_array;
    njs_array_buffer_t  *buffer, *new_buffer;

    this = njs_argument(args, 0);
    if (njs_slow_path(!njs_is_typed_array(this))) {
        njs_type_error(vm, "this is not a typed array");
        return NJS_ERROR;
    }

    array = njs_typed_array(this);
    length = njs_typed_array_length(array);
    buffer = njs_typed_array_buffer(array);
    if (njs_slow_path(copy && njs_is_detached_buffer(buffer))) {
        njs_type_error(vm, "detached buffer");
        return NJS_ERROR;
    }

    ret = njs_value_to_integer(vm, njs_arg(args, nargs, 1), &start);
    if (njs_slow_path(ret != NJS_OK)) {
        njs_range_error(vm, "invalid start");
        return NJS_ERROR;
    }

    start = (start < 0) ? njs_max(length + start, 0) : njs_min(start, length);

    value = njs_arg(args, nargs, 2);

    if (njs_is_undefined(value)) {
        end = length;

    } else {
        ret = njs_value_to_integer(vm, value, &end);
        if (njs_slow_path(ret != NJS_OK)) {
            njs_range_error(vm, "invalid end");
            return NJS_ERROR;
        }
    }

    end = (end < 0) ? njs_max(length + end, 0) : njs_min(end, length);

    element_size = njs_typed_array_element_size(array->type);

    if (copy) {
        count = njs_max(end - start, 0);
        njs_set_number(&arguments[0], count);

        ret = njs_typed_array_species_create(vm, this,
                                             njs_value_arg(arguments), 1,
                                             &vm->retval);
        if (njs_slow_path(ret != NJS_OK)) {
            return ret;
        }

        if (count == 0) {
            return NJS_OK;
        }

        if (njs_slow_path(njs_is_detached_buffer(buffer))) {
            njs_type_error(vm, "detached buffer");
            return NJS_ERROR;
        }

        new_array = njs_typed_array(&vm->retval);
        new_buffer = njs_typed_array_buffer(new_array);
        element_size = njs_typed_array_element_size(array->type);

        if (njs_fast_path(array->type == new_array->type)) {
            start = start * element_size;
            count = count * element_size;

            memcpy(&new_buffer->u.u8[0], &buffer->u.u8[start], count);

        } else {
            for (i = 0; i < count; i++) {
                njs_typed_array_prop_set(vm, new_array, i,
                                        njs_typed_array_prop(array, i + start));
            }
        }

        return NJS_OK;
    }

    offset = array->offset * element_size;
    offset += start * element_size;

    njs_set_array_buffer(&arguments[0], buffer);
    njs_set_number(&arguments[1], offset);
    njs_set_number(&arguments[2], njs_max(end - start, 0));

    return njs_typed_array_species_create(vm, this, njs_value_arg(arguments), 3,
                                          &vm->retval);
}