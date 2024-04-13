njs_module_require(njs_vm_t *vm, njs_value_t *args, njs_uint_t nargs,
    njs_index_t unused)
{
    njs_int_t    ret;
    njs_str_t    name;
    njs_mod_t    *module;
    njs_value_t  *path;

    if (nargs < 2) {
        njs_type_error(vm, "missing path");
        return NJS_ERROR;
    }

    path = njs_argument(args, 1);
    ret = njs_value_to_string(vm, path, path);
    if (njs_slow_path(ret != NJS_OK)) {
        return ret;
    }

    njs_string_get(path, &name);

    module = njs_module_find(vm, &name, 0);
    if (njs_slow_path(module == NULL)) {
        njs_error(vm, "Cannot find module \"%V\"", &name);

        return NJS_ERROR;
    }

    njs_value_assign(&vm->retval, &module->value);

    return NJS_OK;
}