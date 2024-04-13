njs_typed_array_constructor_intrinsic(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t unused)
{
    njs_type_error(vm, "Abstract class TypedArray not directly constructable");

    return NJS_ERROR;
}