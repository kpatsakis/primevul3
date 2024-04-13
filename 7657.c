njs_typed_array_prototype_iterator_obj(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t kind)
{
    njs_value_t        *this;
    njs_typed_array_t  *array;

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

    return njs_array_iterator_create(vm, this, &vm->retval, kind);
}