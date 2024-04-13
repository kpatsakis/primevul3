njs_typed_array_prototype_length(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t unused)
{
    uint32_t           length;
    njs_value_t        *this;
    njs_typed_array_t  *array;

    this = njs_argument(args, 0);
    if (!njs_is_typed_array(this)) {
        njs_type_error(vm, "Method TypedArray.prototype.length called "
                       "on incompatible receiver");
        return NJS_ERROR;
    }

    array = njs_typed_array(this);
    length = njs_typed_array_length(array);

    if (njs_slow_path(njs_is_detached_buffer(array->buffer))) {
        length = 0;
    }

    njs_set_number(&vm->retval, length);

    return NJS_OK;
}