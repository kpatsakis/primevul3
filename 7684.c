njs_typed_array_get_this(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t unused)
{
    vm->retval = args[0];

    return NJS_OK;
}