MagickExport char *CanonicalXMLContent(const char *content,
  const MagickBooleanType pedantic)
{
  char
    *base64,
    *canonical_content;

  register const unsigned char
    *p;

  register ssize_t
    i;

  size_t
    extent,
    length;

  unsigned char
    *utf8;

  utf8=ConvertLatin1ToUTF8((const unsigned char *) content);
  if (utf8 == (unsigned char *) NULL)
    return((char *) NULL);
  for (p=utf8; *p != '\0'; p++)
    if ((*p < 0x20) && (*p != 0x09) && (*p != 0x0a) && (*p != 0x0d))
      break;
  if (*p != '\0')
    {
      /*
        String is binary, base64-encode it.
      */
      base64=Base64Encode(utf8,strlen((char *) utf8),&length);
      utf8=(unsigned char *) RelinquishMagickMemory(utf8);
      if (base64 == (char *) NULL)
        return((char *) NULL);
      canonical_content=AcquireString("<base64>");
      (void) ConcatenateString(&canonical_content,base64);
      base64=DestroyString(base64);
      (void) ConcatenateString(&canonical_content,"</base64>");
      return(canonical_content);
    }
  /*
    Substitute predefined entities.
  */
  i=0;
  canonical_content=AcquireString((char *) NULL);
  extent=MaxTextExtent;
  for (p=utf8; *p != '\0'; p++)
  {
    if ((i+MaxTextExtent) > (ssize_t) extent)
      {
        extent+=MaxTextExtent;
        canonical_content=(char *) ResizeQuantumMemory(canonical_content,extent,
          sizeof(*canonical_content));
        if (canonical_content == (char *) NULL)
          return(canonical_content);
      }
    switch (*p)
    {
      case '&':
      {
        i+=FormatLocaleString(canonical_content+i,extent,"&amp;");
        break;
      }
      case '<':
      {
        i+=FormatLocaleString(canonical_content+i,extent,"&lt;");
        break;
      }
      case '>':
      {
        i+=FormatLocaleString(canonical_content+i,extent,"&gt;");
        break;
      }
      case '"':
      {
        i+=FormatLocaleString(canonical_content+i,extent,"&quot;");
        break;
      }
      case '\n':
      {
        if (pedantic == MagickFalse)
          {
            canonical_content[i++]=(char) (*p);
            break;
          }
        i+=FormatLocaleString(canonical_content+i,extent,"&#xA;");
        break;
      }
      case '\t':
      {
        if (pedantic == MagickFalse)
          {
            canonical_content[i++]=(char) (*p);
            break;
          }
        i+=FormatLocaleString(canonical_content+i,extent,"&#x9;");
        break;
      }
      case '\r':
      {
        i+=FormatLocaleString(canonical_content+i,extent,"&#xD;");
        break;
      }
      default:
      {
        canonical_content[i++]=(char) (*p);
        break;
      }
    }
  }
  canonical_content[i]='\0';
  utf8=(unsigned char *) RelinquishMagickMemory(utf8);
  return(canonical_content);
}
