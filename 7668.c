njs_typed_array_prop_set(njs_vm_t *vm, njs_typed_array_t *array, uint32_t index,
    double v)
{
    int8_t              i8;
    int16_t             i16;
    int32_t             i32;
    njs_array_buffer_t  *buffer;

    buffer = array->buffer;
    index += array->offset;

    njs_assert(!buffer->object.shared);

    switch (array->type) {
    case NJS_OBJ_TYPE_UINT8_CLAMPED_ARRAY:
        if (isnan(v) || v < 0) {
            v = 0;
        } else if (v > 255) {
            v = 255;
        }

        buffer->u.u8[index] = lrint(v);

        break;

    case NJS_OBJ_TYPE_UINT8_ARRAY:
    case NJS_OBJ_TYPE_INT8_ARRAY:
        i8 = njs_number_to_int32(v);
        buffer->u.u8[index] = i8;
        break;

    case NJS_OBJ_TYPE_UINT16_ARRAY:
    case NJS_OBJ_TYPE_INT16_ARRAY:
        i16 = njs_number_to_int32(v);
        buffer->u.u16[index] = i16;
        break;

    case NJS_OBJ_TYPE_UINT32_ARRAY:
    case NJS_OBJ_TYPE_INT32_ARRAY:
        i32 = njs_number_to_int32(v);
        buffer->u.u32[index] = i32;
        break;

    case NJS_OBJ_TYPE_FLOAT32_ARRAY:
        buffer->u.f32[index] = v;
        break;

    default:

        /* NJS_OBJ_TYPE_FLOAT64_ARRAY. */

        buffer->u.f64[index] = v;
    }
}