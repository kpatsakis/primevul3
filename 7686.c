njs_typed_array_prototype_set(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t unused)
{
    double              num;
    int64_t             i, length, src_length, offset;
    njs_int_t           ret;
    njs_value_t         *this, *src, *value, prop;
    njs_array_t         *array;
    njs_typed_array_t   *self, *src_tarray;
    njs_array_buffer_t  *buffer;

    this = njs_argument(args, 0);
    if (njs_slow_path(!njs_is_typed_array(this))) {
        njs_type_error(vm, "this is not a typed array");
        return NJS_ERROR;
    }

    self = njs_typed_array(this);
    src = njs_arg(args, nargs, 1);
    value = njs_arg(args, nargs, 2);

    ret = njs_value_to_integer(vm, value, &offset);
    if (njs_slow_path(ret != NJS_OK)) {
        return NJS_ERROR;
    }

    if (njs_slow_path(offset < 0)) {
        njs_range_error(vm, "offset is out of bounds");
        return NJS_ERROR;
    }

    buffer = njs_typed_array_writable(vm, self);
    if (njs_slow_path(buffer == NULL)) {
        return NJS_ERROR;
    }

    length = njs_typed_array_length(self);

    if (njs_is_typed_array(src)) {
        src_tarray = njs_typed_array(src);
        if (njs_slow_path(njs_is_detached_buffer(src_tarray->buffer))) {
            njs_type_error(vm, "detached buffer");
            return NJS_ERROR;
        }

        src_length = njs_typed_array_length(src_tarray);

        if (njs_slow_path((src_length > length)
                          || (offset > length - src_length)))
        {
            njs_range_error(vm, "source is too large");
            return NJS_ERROR;
        }

        length = njs_min(njs_typed_array_length(src_tarray), length - offset);

        for (i = 0; i < length; i++) {
            njs_typed_array_prop_set(vm, self, offset + i,
                                     njs_typed_array_prop(src_tarray, i));
        }

    } else {
        if (njs_is_fast_array(src)) {
            array = njs_array(src);
            src_length = array->length;

            if (njs_slow_path((src_length > length)
                              || (offset > length - src_length)))
            {
                njs_range_error(vm, "source is too large");
                return NJS_ERROR;
            }

            length = njs_min(array->length, length - offset);

            for (i = 0; i < length; i++) {
                ret = njs_value_to_number(vm, &array->start[i], &num);
                if (ret == NJS_OK) {
                    njs_typed_array_prop_set(vm, self, offset + i, num);
                }
            }

            goto done;
        }

        ret = njs_value_to_object(vm, src);
        if (njs_slow_path(ret != NJS_OK)) {
            return ret;
        }

        ret = njs_object_length(vm, src, &src_length);
        if (njs_slow_path(ret == NJS_ERROR)) {
            return ret;
        }

        if (njs_slow_path((src_length > length)
                          || (offset > length - src_length)))
        {
            njs_range_error(vm, "source is too large");
            return NJS_ERROR;
        }

        length = njs_min(src_length, length - offset);

        for (i = 0; i < length; i++) {
            ret = njs_value_property_i64(vm, src, i, &prop);
            if (njs_slow_path(ret == NJS_ERROR)) {
                return NJS_ERROR;
            }

            num = NAN;

            if (ret == NJS_OK) {
                ret = njs_value_to_number(vm, &prop, &num);
                if (njs_slow_path(ret == NJS_ERROR)) {
                    return NJS_ERROR;
                }
            }

            if (njs_slow_path(njs_is_detached_buffer(buffer))) {
                njs_type_error(vm, "detached buffer");
                return NJS_ERROR;
            }

            njs_typed_array_prop_set(vm, self, offset + i, num);
        }
    }

done:

    njs_set_undefined(&vm->retval);

    return NJS_OK;
}