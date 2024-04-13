static String storageDirectory(DWORD pathIdentifier)
{
    Vector<UChar> buffer(MAX_PATH);
    if (FAILED(SHGetFolderPathW(0, pathIdentifier | CSIDL_FLAG_CREATE, 0, 0, buffer.data())))
        return String();
    buffer.resize(wcslen(buffer.data()));
    String directory = String::adopt(buffer);

    DEFINE_STATIC_LOCAL(String, companyNameDirectory, (ASCIILiteral("Apple Computer\\")));
    directory = pathByAppendingComponent(directory, companyNameDirectory + bundleName());
    if (!makeAllDirectories(directory))
        return String();

    return directory;
}
