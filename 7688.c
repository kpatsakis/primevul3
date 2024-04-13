njs_typed_array_to_chain(njs_vm_t *vm, njs_chb_t *chain,
    njs_typed_array_t *array, njs_value_t *sep)
{
    size_t             size, length, arr_length;
    uint32_t           i;
    njs_string_prop_t  separator;

    if (sep == NULL) {
        sep = njs_value_arg(&njs_string_comma);
    }

    (void) njs_string_prop(&separator, sep);

    arr_length = njs_typed_array_length(array);

    if (arr_length == 0) {
        return 0;
    }

    for (i = 0; i < arr_length; i++) {
        njs_number_to_chain(vm, chain, njs_typed_array_prop(array, i));
        njs_chb_append(chain, separator.start, separator.size);
    }

    njs_chb_drop(chain, separator.size);

    size = njs_chb_size(chain);

    if (njs_utf8_length(separator.start, separator.size) >= 0) {
        length = size - (separator.size - separator.length) * (arr_length - 1);

    } else {
        length = 0;
    }

    return length;
}