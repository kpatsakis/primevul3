njs_typed_array_prototype_byte_offset(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t unused)
{
    size_t             byte_offset;
    njs_value_t        *this;
    njs_typed_array_t  *array;

    this = njs_argument(args, 0);
    if (!njs_is_typed_array(this) && !njs_is_data_view(this)) {
        njs_type_error(vm, "Method TypedArray.prototype.byteOffset called "
                       "on incompatible receiver");
        return NJS_ERROR;
    }

    array = njs_typed_array(this);
    byte_offset = njs_typed_array_offset(array);

    if (njs_slow_path(njs_is_detached_buffer(array->buffer))) {
        if (njs_is_data_view(this)) {
            njs_type_error(vm, "detached buffer");
            return NJS_ERROR;
        }

        byte_offset = 0;
    }

    njs_set_number(&vm->retval, byte_offset);

    return NJS_OK;
}