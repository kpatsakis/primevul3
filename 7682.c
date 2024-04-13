njs_typed_array_prototype_buffer(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t unused)
{
    njs_value_t        *this;
    njs_typed_array_t  *array;

    this = njs_argument(args, 0);
    if (!njs_is_typed_array(this) && !njs_is_data_view(this)) {
        njs_type_error(vm, "Method TypedArray.prototype.buffer called "
                       "on incompatible receiver");
        return NJS_ERROR;
    }

    array = njs_typed_array(this);

    njs_set_array_buffer(&vm->retval, njs_typed_array_buffer(array));

    return NJS_OK;
}