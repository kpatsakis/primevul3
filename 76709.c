Vector<String> listDirectory(const String& directory, const String& filter)
{
    Vector<String> entries;

    PathWalker walker(directory, filter);
    if (!walker.isValid())
        return entries;

    do {
        if (walker.data().dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        entries.append(directory + "\\" + reinterpret_cast<const UChar*>(walker.data().cFileName));
    } while (walker.step());

    return entries;
}
