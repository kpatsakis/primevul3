static void* getProcAddress(const char* procName)
{
    return reinterpret_cast<void*>(QOpenGLContext::currentContext()->getProcAddress(procName));
}
