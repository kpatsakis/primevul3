bool JSArray::deleteProperty(ExecState* exec, const Identifier& propertyName)
{
    bool isArrayIndex;
    unsigned i = propertyName.toArrayIndex(isArrayIndex);
    if (isArrayIndex)
        return deleteProperty(exec, i);

    if (propertyName == exec->propertyNames().length)
        return false;

    return JSObject::deleteProperty(exec, propertyName);
}
