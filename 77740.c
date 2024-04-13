__inline double png_strtod(png_structp png_ptr, PNG_CONST char *nptr,
    char **endptr)
{
   double result = 0;
   int len;
   wchar_t *str, *end;

   len = MultiByteToWideChar(CP_ACP, 0, nptr, -1, NULL, 0);
   str = (wchar_t *)png_malloc(png_ptr, len * png_sizeof(wchar_t));
   if ( NULL != str )
   {
      MultiByteToWideChar(CP_ACP, 0, nptr, -1, str, len);
      result = wcstod(str, &end);
      len = WideCharToMultiByte(CP_ACP, 0, end, -1, NULL, 0, NULL, NULL);
      *endptr = (char *)nptr + (png_strlen(nptr) - len + 1);
      png_free(png_ptr, str);
   }
   return result;
}
