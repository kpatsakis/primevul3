njs_typed_array_prototype_byte_length(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t unused)
{
    size_t             byte_length;
    njs_value_t        *this;
    njs_typed_array_t  *array;

    this = njs_argument(args, 0);
    if (!njs_is_typed_array(this) && !njs_is_data_view(this)) {
        njs_type_error(vm, "Method TypedArray.prototype.byteLength called "
                       "on incompatible receiver");
        return NJS_ERROR;
    }

    array = njs_typed_array(this);
    byte_length = array->byte_length;

    if (njs_slow_path(njs_is_detached_buffer(array->buffer))) {
        if (njs_is_data_view(this)) {
            njs_type_error(vm, "detached buffer");
            return NJS_ERROR;
        }

        byte_length = 0;
    }

    njs_set_number(&vm->retval, byte_length);

    return NJS_OK;
}