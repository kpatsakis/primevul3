static String bundleName()
{
    DEFINE_STATIC_LOCAL(String, name, (ASCIILiteral("WebKit")));

#if USE(CF)
    static bool initialized;

    if (!initialized) {
        initialized = true;

        if (CFBundleRef bundle = CFBundleGetMainBundle())
            if (CFTypeRef bundleExecutable = CFBundleGetValueForInfoDictionaryKey(bundle, kCFBundleExecutableKey))
                if (CFGetTypeID(bundleExecutable) == CFStringGetTypeID())
                    name = reinterpret_cast<CFStringRef>(bundleExecutable);
    }
#endif

    return name;
}
