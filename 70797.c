stb_vorbis * stb_vorbis_open_filename(const char *filename, int *error, const stb_vorbis_alloc *alloc)
{
   FILE *f;
#if defined(_WIN32) && defined(__STDC_WANT_SECURE_LIB__)
   if (0 != fopen_s(&f, filename, "rb"))
      f = NULL;
#else
   f = fopen(filename, "rb");
#endif
   if (f) 
      return stb_vorbis_open_file(f, TRUE, error, alloc);
   if (error) *error = VORBIS_file_open_failure;
   return NULL;
}
