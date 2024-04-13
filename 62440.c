MagickExport void ListMagickVersion(FILE *file)
{
  (void) FormatLocaleFile(file,"Version: %s\n",
    GetMagickVersion((size_t *) NULL));
  (void) FormatLocaleFile(file,"Copyright: %s\n",GetMagickCopyright());
  (void) FormatLocaleFile(file,"License: %s\n",GetMagickLicense());
  (void) FormatLocaleFile(file,"Features: %s\n",GetMagickFeatures());
  (void) FormatLocaleFile(file,"Delegates (built-in): %s\n\n",
    GetMagickDelegates());
}
