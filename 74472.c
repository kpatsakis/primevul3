static int matchFunc(const char*)
{
    return XMLTokenizerScope::currentDocLoader && currentThread() == libxmlLoaderThread;
}
