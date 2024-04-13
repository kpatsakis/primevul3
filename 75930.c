PPVarFromNPObject::PPVarFromNPObject(PluginInstance* instance, NPObject* object)
    : var_(ObjectVar::NPObjectToPPVar(instance, object)) {
}
