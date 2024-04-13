njs_typed_array_species_create(njs_vm_t *vm, njs_value_t *exemplar,
    njs_value_t *args, njs_uint_t nargs, njs_value_t *retval)
{
    njs_int_t          ret;
    njs_value_t        constructor;
    njs_typed_array_t  *array;

    array = njs_typed_array(exemplar);

    njs_set_function(&constructor, &vm->constructors[array->type]);

    ret = njs_value_species_constructor(vm, exemplar, &constructor,
                                        &constructor);
    if (njs_slow_path(ret != NJS_OK)) {
        return ret;
    }

    return njs_typed_array_create(vm, &constructor, args, nargs, retval);
}