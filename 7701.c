njs_typed_array_get_string_tag(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t unused)
{
    njs_value_t  *this;

    static const njs_value_t  *tags[NJS_OBJ_TYPE_TYPED_ARRAY_SIZE] = {
        &njs_typed_array_uint8_tag,
        &njs_typed_array_uint8_clamped_tag,
        &njs_typed_array_int8_tag,
        &njs_typed_array_uint16_tag,
        &njs_typed_array_int16_tag,
        &njs_typed_array_uint32_tag,
        &njs_typed_array_int32_tag,
        &njs_typed_array_float32_tag,
        &njs_typed_array_float64_tag,
    };

    this = njs_argument(args, 0);

    if (!njs_is_typed_array(this)) {
        njs_set_undefined(&vm->retval);
        return NJS_OK;
    }

    vm->retval = *tags[njs_typed_array_index(njs_typed_array(this)->type)];

    return NJS_OK;
}