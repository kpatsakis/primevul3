njs_typed_array_create(njs_vm_t *vm, njs_value_t *constructor,
    njs_value_t *args, njs_uint_t nargs, njs_value_t *retval)
{
    njs_int_t          ret;
    njs_value_t        this;
    njs_object_t       *object;
    njs_typed_array_t  *array;

    object = njs_function_new_object(vm, constructor);
    if (njs_slow_path(object == NULL)) {
        return NJS_ERROR;
    }

    njs_set_object(&this, object);

    ret = njs_function_call2(vm, njs_function(constructor), &this, args,
                             nargs, retval, 1);
    if (njs_slow_path(ret != NJS_OK)) {
        return NJS_ERROR;
    }

    if (njs_slow_path(!njs_is_typed_array(retval))) {
        njs_type_error(vm, "Derived TypedArray constructor "
                       "returned not a typed array");
        return NJS_ERROR;
    }

    array = njs_typed_array(retval);
    if (njs_slow_path(njs_is_detached_buffer(array->buffer))) {
        njs_type_error(vm, "detached buffer");
        return NJS_ERROR;
    }

    if (njs_slow_path(nargs == 1 && njs_is_number(&args[0])
                      && njs_typed_array_length(array)
                         < njs_number(&args[0])))
    {
        njs_type_error(vm, "Derived TypedArray constructor "
                       "returned too short array");
        return NJS_ERROR;
    }

    return NJS_OK;
}