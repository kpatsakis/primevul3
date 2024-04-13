njs_typed_array_prototype_index_of(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t type)
{
    double              v;
    int64_t             i, i64, from, to, index, increment, offset, length;
    njs_int_t           ret, integer;
    njs_value_t         *this;
    const float         *f32;
    const double        *f64;
    const uint8_t       *u8;
    const uint16_t      *u16;
    const uint32_t      *u32;
    njs_typed_array_t   *array;
    njs_array_buffer_t  *buffer;

    this = njs_argument(args, 0);
    if (njs_slow_path(!njs_is_typed_array(this))) {
        njs_type_error(vm, "this is not a typed array");
        return NJS_ERROR;
    }

    index = -1;
    array = njs_typed_array(this);
    length = njs_typed_array_length(array);

    if (!njs_is_number(njs_arg(args, nargs, 1)) || length == 0) {
        goto done;
    }

    if (type & 2) {
        /* lastIndexOf(). */

        if (nargs > 2) {
            ret = njs_value_to_integer(vm, njs_arg(args, nargs, 2), &from);
            if (njs_slow_path(ret != NJS_OK)) {
                return ret;
            }

        } else {
            from = length - 1;
        }

        if (from >= 0) {
            from = njs_min(from, length - 1);

        } else if (from < 0) {
            from += length;
        }

        to = -1;
        increment = -1;

        if (from <= to) {
            goto done;
        }

    } else {
        /* indexOf(), includes(). */

        ret = njs_value_to_integer(vm, njs_arg(args, nargs, 2), &from);
        if (njs_slow_path(ret != NJS_OK)) {
            return ret;
        }

        if (from < 0) {
            from += length;

            if (from < 0) {
                from = 0;
            }
        }

        to = length;
        increment = 1;

        if (from >= to) {
            goto done;
        }
    }

    if (njs_slow_path(njs_is_detached_buffer(array->buffer))) {
        njs_type_error(vm, "detached buffer");
        return NJS_ERROR;
    }

    v = njs_number(njs_argument(args, 1));

    i64 = v;
    integer = (v == i64);

    buffer = array->buffer;
    offset = array->offset;

    switch (array->type) {
    case NJS_OBJ_TYPE_INT8_ARRAY:
        if (integer && ((int8_t) i64 == i64)) {
            goto search8;
        }

        break;

    case NJS_OBJ_TYPE_UINT8_CLAMPED_ARRAY:
    case NJS_OBJ_TYPE_UINT8_ARRAY:
        if (integer && ((uint8_t) i64 == i64)) {
search8:
            u8 = &buffer->u.u8[0];
            for (i = from; i != to; i += increment) {
                if (u8[offset + i] == (uint8_t) i64) {
                    index = i;
                    break;
                }
            }
        }

        break;

    case NJS_OBJ_TYPE_INT16_ARRAY:
        if (integer && ((int16_t) i64 == i64)) {
            goto search16;
        }

        break;

    case NJS_OBJ_TYPE_UINT16_ARRAY:
        if (integer && ((uint16_t) i64 == i64)) {
search16:
            u16 = &buffer->u.u16[0];
            for (i = from; i != to; i += increment) {
                if (u16[offset + i] == (uint16_t) i64) {
                    index = i;
                    break;
                }
            }
        }

        break;

    case NJS_OBJ_TYPE_INT32_ARRAY:
        if (integer && ((int32_t) i64 == i64)) {
            goto search32;
        }

        break;

    case NJS_OBJ_TYPE_UINT32_ARRAY:
        if (integer && ((uint32_t) i64 == i64)) {
search32:
            u32 = &buffer->u.u32[0];
            for (i = from; i != to; i += increment) {
                if (u32[offset + i] == (uint32_t) i64) {
                    index = i;
                    break;
                }
            }
        }

        break;

    case NJS_OBJ_TYPE_FLOAT32_ARRAY:
        f32 = &buffer->u.f32[0];

        if (((float) v == v)) {
            for (i = from; i != to; i += increment) {
                if (f32[offset + i] == (float) v) {
                    index = i;
                    break;
                }
            }

        } else if ((type & 1) && isnan(v)) {
            /* includes() handles NaN. */

            for (i = from; i != to; i += increment) {
                if (isnan(f32[offset + i])) {
                    index = i;
                    break;
                }
            }
        }

        break;

    default:

        /* NJS_OBJ_TYPE_FLOAT64_ARRAY. */

        f64 = &buffer->u.f64[0];

        if ((type & 1) && isnan(v)) {
            /* includes() handles NaN. */

            for (i = from; i != to; i += increment) {
                if (isnan(f64[offset + i])) {
                    index = i;
                    break;
                }
            }

        } else {
            for (i = from; i != to; i += increment) {
                if (f64[offset + i] == v) {
                    index = i;
                    break;
                }
            }
        }
    }

done:

    /* Default values. */

    if (type & 1) {
        njs_set_boolean(&vm->retval, index != -1);

    } else {
        njs_set_number(&vm->retval, index);
    }

    return NJS_OK;
}