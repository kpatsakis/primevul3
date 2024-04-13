static void JBIGEncode(unsigned char *pixels,size_t length,void *data)
{
  Image
    *image;

  image=(Image *) data;
  (void) WriteBlob(image,length,pixels);
}
