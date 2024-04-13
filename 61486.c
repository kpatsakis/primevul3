static char *ConvertUTF16ToUTF8(const char *content,size_t *length)
{
  char
    *utf8;

  int
    bits,
    byte,
    c,
    encoding;

  register ssize_t
    i;

  size_t
    extent;

  ssize_t
    j;

  utf8=(char *) AcquireQuantumMemory(*length+1,sizeof(*utf8));
  if (utf8 == (char *) NULL)
    return((char *) NULL);
  encoding=(*content == '\xFE') ? 1 : (*content == '\xFF') ? 0 : -1;
  if (encoding == -1)
    {
      /*
        Already UTF-8.
      */
      (void) CopyMagickMemory(utf8,content,*length*sizeof(*utf8));
      utf8[*length]='\0';
      return(utf8);
    }
  j=0;
  extent=(*length);
  for (i=2; i < (ssize_t) (*length-1); i+=2)
  {
    c=(encoding != 0) ? ((content[i] & 0xff) << 8) | (content[i+1] & 0xff) :
      ((content[i+1] & 0xff) << 8) | (content[i] & 0xff);
    if ((c >= 0xd800) && (c <= 0xdfff) && ((i+=2) < (ssize_t) (*length-1)))
      {
        byte=(encoding != 0) ? ((content[i] & 0xff) << 8) |
          (content[i+1] & 0xff) : ((content[i+1] & 0xff) << 8) |
          (content[i] & 0xff);
        c=(((c & 0x3ff) << 10) | (byte & 0x3ff))+0x10000;
      }
    if ((size_t) (j+MaxTextExtent) > extent)
      {
        extent=(size_t) j+MaxTextExtent;
        utf8=(char *) ResizeQuantumMemory(utf8,extent,sizeof(*utf8));
        if (utf8 == (char *) NULL)
          return(utf8);
      }
    if (c < 0x80)
      {
        utf8[j]=c;
        j++;
        continue;
      }
    /*
      Multi-byte UTF-8 sequence.
    */
    byte=c;
    for (bits=0; byte != 0; byte/=2)
      bits++;
    bits=(bits-2)/5;
    utf8[j++]=(0xFF << (7-bits)) | (c >> (6*bits));
    while (bits != 0)
    {
      bits--;
      utf8[j]=0x80 | ((c >> (6*bits)) & 0x3f);
      j++;
    }
  }
  *length=(size_t) j;
  utf8=(char *) ResizeQuantumMemory(utf8,*length,sizeof(*utf8));
  if (utf8 != (char *) NULL)
    utf8[*length]='\0';
  return(utf8);
}
