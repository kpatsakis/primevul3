njs_typed_array_constructor(njs_vm_t *vm, njs_value_t *args, njs_uint_t nargs,
    njs_index_t magic)
{
    njs_typed_array_t  *array;

    if (!vm->top_frame->ctor) {
        njs_type_error(vm, "Constructor of TypedArray requires 'new'");
        return NJS_ERROR;
    }

    array = njs_typed_array_alloc(vm, &args[1], nargs - 1, 1, magic);
    if (njs_slow_path(array == NULL)) {
        return NJS_ERROR;
    }

    njs_set_typed_array(&vm->retval, array);

    return NJS_OK;
}