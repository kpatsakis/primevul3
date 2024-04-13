njs_data_view_prototype_set(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t type)
{
    double              v;
    uint8_t             *u8;
    uint32_t            u32;
    uint64_t            index;
    njs_int_t           ret;
    njs_bool_t          swap;
    njs_value_t         *this;
    njs_conv_f32_t      conv_f32;
    njs_conv_f64_t      conv_f64;
    njs_data_view_t     *view;
    njs_array_buffer_t  *buffer;

    this = njs_argument(args, 0);
    if (njs_slow_path(!njs_is_data_view(this))) {
        njs_type_error(vm, "this is not a DataView");
        return NJS_ERROR;
    }

    ret = njs_value_to_index(vm, njs_arg(args, nargs, 1), &index);
    if (njs_slow_path(ret != NJS_OK)) {
        return NJS_ERROR;
    }

    ret = njs_value_to_number(vm, njs_arg(args, nargs, 2), &v);
    if (njs_slow_path(ret != NJS_OK)) {
        return NJS_ERROR;
    }

    swap = njs_bool(njs_arg(args, nargs, 3));

#if NJS_HAVE_LITTLE_ENDIAN
    swap = !swap;
#endif

    view = njs_data_view(this);
    if (njs_slow_path(njs_is_detached_buffer(view->buffer))) {
        njs_type_error(vm, "detached buffer");
        return NJS_ERROR;
    }

    if (njs_typed_array_element_size(type) + index > view->byte_length) {
        njs_range_error(vm, "index %uL is outside the bound of the buffer",
                        index);
        return NJS_ERROR;
    }

    buffer = view->buffer;
    u8 = &buffer->u.u8[index + view->offset];

    switch (type) {
    case NJS_OBJ_TYPE_UINT8_ARRAY:
    case NJS_OBJ_TYPE_INT8_ARRAY:
        *u8 = njs_number_to_int32(v);
        break;

    case NJS_OBJ_TYPE_UINT16_ARRAY:
    case NJS_OBJ_TYPE_INT16_ARRAY:
        u32 = (uint16_t) njs_number_to_int32(v);

        if (swap) {
            u32 = njs_bswap_u16(u32);
        }

        njs_set_u16(u8, u32);
        break;

    case NJS_OBJ_TYPE_UINT32_ARRAY:
    case NJS_OBJ_TYPE_INT32_ARRAY:
        u32 = njs_number_to_int32(v);

        if (swap) {
            u32 = njs_bswap_u32(u32);
        }

        njs_set_u32(u8, u32);
        break;

    case NJS_OBJ_TYPE_FLOAT32_ARRAY:
        conv_f32.f = (float) v;

        if (swap) {
            conv_f32.u = njs_bswap_u32(conv_f32.u);
        }

        njs_set_u32(u8, conv_f32.u);
        break;

    default:
        /* NJS_OBJ_TYPE_FLOAT64_ARRAY. */

        conv_f64.f = v;

        if (swap) {
            conv_f64.u = njs_bswap_u64(conv_f64.u);
        }

        njs_set_u64(u8, conv_f64.u);
    }

    njs_set_undefined(&vm->retval);

    return NJS_OK;
}