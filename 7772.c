njs_parser_module(njs_parser_t *parser, njs_str_t *name)
{
    njs_mod_t            *module;
    njs_vm_t             *vm;
    njs_external_ptr_t   external;
    njs_module_loader_t  loader;

    vm = parser->vm;

    if (name->length == 0) {
        njs_parser_syntax_error(parser, "Cannot find module \"%V\"", name);
        return NULL;
    }

    module = njs_module_find(vm, name, 1);
    if (module != NULL) {
        goto done;
    }

    external = parser;
    loader = njs_default_module_loader;

    if (vm->options.ops != NULL && vm->options.ops->module_loader != NULL) {
        loader = vm->options.ops->module_loader;
        external = vm->external;
    }

    module = loader(vm, external, name);
    if (module == NULL) {
        njs_parser_syntax_error(parser, "Cannot find module \"%V\"", name);
        return NULL;
    }

done:

    if (module->index == 0) {
        module->index = vm->shared->module_items++;
    }

    return module;
}