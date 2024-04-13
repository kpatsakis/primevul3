njs_typed_array_prototype_fill(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t unused)
{
    float               f32;
    int8_t              i8;
    double              num;
    int16_t             i16;
    int32_t             i32;
    uint8_t             u8;
    int64_t             start, end, offset;
    uint32_t            i, length;
    njs_int_t           ret;
    njs_value_t         *this, *setval, lvalue;
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

    setval = njs_lvalue_arg(&lvalue, args, nargs, 1);
    ret = njs_value_to_number(vm, setval, &num);
    if (njs_slow_path(ret != NJS_OK)) {
        return NJS_ERROR;
    }

    ret = njs_value_to_integer(vm, njs_arg(args, nargs, 2), &start);
    if (njs_slow_path(ret != NJS_OK)) {
        return ret;
    }

    start = (start < 0) ? njs_max(length + start, 0) : njs_min(start, length);

    if (njs_is_undefined(njs_arg(args, nargs, 3))) {
        end = length;

    } else {
        ret = njs_value_to_integer(vm, njs_arg(args, nargs, 3), &end);
        if (njs_slow_path(ret != NJS_OK)) {
            return ret;
        }
    }

    end = (end < 0) ? njs_max(length + end, 0) : njs_min(end, length);

    buffer = njs_typed_array_writable(vm, array);
    if (njs_slow_path(buffer == NULL)) {
        return NJS_ERROR;
    }

    njs_set_typed_array(&vm->retval, array);

    offset = array->offset;

    switch (array->type) {
    case NJS_OBJ_TYPE_UINT8_CLAMPED_ARRAY:
        if (isnan(num) || num < 0) {
            u8 = 0;

        } else if (num > 255) {
            u8 = 255;

        } else {
            u8 = lrint(num);
        }

        if (start < end) {
            memset(&buffer->u.u8[start + offset], u8, end - start);
        }

        break;

    case NJS_OBJ_TYPE_UINT8_ARRAY:
    case NJS_OBJ_TYPE_INT8_ARRAY:
        i8 = njs_number_to_int32(num);

        if (start < end) {
            memset(&buffer->u.u8[start + offset], i8, end - start);
        }

        break;

    case NJS_OBJ_TYPE_UINT16_ARRAY:
    case NJS_OBJ_TYPE_INT16_ARRAY:
        i16 = njs_number_to_int32(num);

        for (i = start; i < end; i++) {
            buffer->u.u16[i + offset] = i16;
        }

        break;

    case NJS_OBJ_TYPE_UINT32_ARRAY:
    case NJS_OBJ_TYPE_INT32_ARRAY:
        i32 = njs_number_to_int32(num);

        for (i = start; i < end; i++) {
            buffer->u.u32[i + offset] = i32;
        }

        break;

    case NJS_OBJ_TYPE_FLOAT32_ARRAY:
        f32 = num;

        for (i = start; i < end; i++) {
            buffer->u.f32[i + offset] = f32;
        }

        break;

    default:

        /* NJS_OBJ_TYPE_FLOAT64_ARRAY. */

        for (i = start; i < end; i++) {
            buffer->u.f64[i + offset] = num;
        }
    }

    return NJS_OK;
}