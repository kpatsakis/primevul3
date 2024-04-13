njs_typed_array_prototype_reverse(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t unused)
{
    double              *f64;
    uint8_t             *u8;
    int64_t             i, length;
    uint16_t            *u16;
    uint32_t            *u32;
    njs_value_t         *this;
    njs_typed_array_t   *array;
    njs_array_buffer_t  *buffer;

    this = njs_argument(args, 0);
    if (njs_slow_path(!njs_is_typed_array(this))) {
        njs_type_error(vm, "this is not a typed array");
        return NJS_ERROR;
    }

    array = njs_typed_array(this);
    length = njs_typed_array_length(array);

    buffer = njs_typed_array_writable(vm, array);
    if (njs_slow_path(buffer == NULL)) {
        return NJS_ERROR;
    }

    switch (array->type) {
    case NJS_OBJ_TYPE_UINT8_ARRAY:
    case NJS_OBJ_TYPE_UINT8_CLAMPED_ARRAY:
    case NJS_OBJ_TYPE_INT8_ARRAY:
        u8 = &buffer->u.u8[array->offset];

        for (i = 0; i < length / 2; i++) {
            njs_swap_u8(&u8[i], &u8[length - i - 1], 0);
        }

        break;

    case NJS_OBJ_TYPE_INT16_ARRAY:
    case NJS_OBJ_TYPE_UINT16_ARRAY:
        u16 = &buffer->u.u16[array->offset];

        for (i = 0; i < length / 2; i++) {
            njs_swap_u16(&u16[i], &u16[length - i - 1], 0);
        }

        break;

    case NJS_OBJ_TYPE_INT32_ARRAY:
    case NJS_OBJ_TYPE_UINT32_ARRAY:
    case NJS_OBJ_TYPE_FLOAT32_ARRAY:
        u32 = &buffer->u.u32[array->offset];

        for (i = 0; i < length / 2; i++) {
            njs_swap_u32(&u32[i], &u32[length - i - 1], 0);
        }

        break;

    default:

        /* NJS_OBJ_TYPE_FLOAT64_ARRAY. */

        f64 = &buffer->u.f64[array->offset];

        for (i = 0; i < length / 2; i++) {
            njs_swap_u64(&f64[i], &f64[length - i - 1], 0);
        }
    }

    njs_set_typed_array(&vm->retval, array);

    return NJS_OK;
}