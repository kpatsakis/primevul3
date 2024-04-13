bool unloadModule(PlatformModule module)
{
    return ::FreeLibrary(module);
}
