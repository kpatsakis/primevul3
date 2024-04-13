static MagickBooleanType WritePSImage(const ImageInfo *image_info,Image *image)
{
#define WriteRunlengthPacket(image,pixel,length,p) \
{ \
  if ((image->matte != MagickFalse) && (length != 0) &&\
      (GetPixelOpacity(p) == (Quantum) TransparentOpacity)) \
    { \
      q=PopHexPixel(hex_digits,0xff,q); \
      q=PopHexPixel(hex_digits,0xff,q); \
      q=PopHexPixel(hex_digits,0xff,q); \
    } \
  else \
    { \
      q=PopHexPixel(hex_digits,ScaleQuantumToChar(pixel.red),q); \
      q=PopHexPixel(hex_digits,ScaleQuantumToChar(pixel.green),q); \
      q=PopHexPixel(hex_digits,ScaleQuantumToChar(pixel.blue),q); \
    } \
  q=PopHexPixel(hex_digits,(size_t) MagickMin(length,0xff),q); \
}

  static const char
    hex_digits[][3] =
    {
      "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B",
      "0C", "0D", "0E", "0F", "10", "11", "12", "13", "14", "15", "16", "17",
      "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", "20", "21", "22", "23",
      "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F",
      "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B",
      "3C", "3D", "3E", "3F", "40", "41", "42", "43", "44", "45", "46", "47",
      "48", "49", "4A", "4B", "4C", "4D", "4E", "4F", "50", "51", "52", "53",
      "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F",
      "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B",
      "6C", "6D", "6E", "6F", "70", "71", "72", "73", "74", "75", "76", "77",
      "78", "79", "7A", "7B", "7C", "7D", "7E", "7F", "80", "81", "82", "83",
      "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
      "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B",
      "9C", "9D", "9E", "9F", "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7",
      "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF", "B0", "B1", "B2", "B3",
      "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF",
      "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB",
      "CC", "CD", "CE", "CF", "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7",
      "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF", "E0", "E1", "E2", "E3",
      "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF",
      "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB",
      "FC", "FD", "FE", "FF"
    },
    PostscriptProlog[] =
      "%%BeginProlog\n"
      "%\n"
      "% Display a color image.  The image is displayed in color on\n"
      "% Postscript viewers or printers that support color, otherwise\n"
      "% it is displayed as grayscale.\n"
      "%\n"
      "/DirectClassPacket\n"
      "{\n"
      "  %\n"
      "  % Get a DirectClass packet.\n"
      "  %\n"
      "  % Parameters:\n"
      "  %   red.\n"
      "  %   green.\n"
      "  %   blue.\n"
      "  %   length: number of pixels minus one of this color (optional).\n"
      "  %\n"
      "  currentfile color_packet readhexstring pop pop\n"
      "  compression 0 eq\n"
      "  {\n"
      "    /number_pixels 3 def\n"
      "  }\n"
      "  {\n"
      "    currentfile byte readhexstring pop 0 get\n"
      "    /number_pixels exch 1 add 3 mul def\n"
      "  } ifelse\n"
      "  0 3 number_pixels 1 sub\n"
      "  {\n"
      "    pixels exch color_packet putinterval\n"
      "  } for\n"
      "  pixels 0 number_pixels getinterval\n"
      "} bind def\n"
      "\n"
      "/DirectClassImage\n"
      "{\n"
      "  %\n"
      "  % Display a DirectClass image.\n"
      "  %\n"
      "  systemdict /colorimage known\n"
      "  {\n"
      "    columns rows 8\n"
      "    [\n"
      "      columns 0 0\n"
      "      rows neg 0 rows\n"
      "    ]\n"
      "    { DirectClassPacket } false 3 colorimage\n"
      "  }\n"
      "  {\n"
      "    %\n"
      "    % No colorimage operator;  convert to grayscale.\n"
      "    %\n"
      "    columns rows 8\n"
      "    [\n"
      "      columns 0 0\n"
      "      rows neg 0 rows\n"
      "    ]\n"
      "    { GrayDirectClassPacket } image\n"
      "  } ifelse\n"
      "} bind def\n"
      "\n"
      "/GrayDirectClassPacket\n"
      "{\n"
      "  %\n"
      "  % Get a DirectClass packet;  convert to grayscale.\n"
      "  %\n"
      "  % Parameters:\n"
      "  %   red\n"
      "  %   green\n"
      "  %   blue\n"
      "  %   length: number of pixels minus one of this color (optional).\n"
      "  %\n"
      "  currentfile color_packet readhexstring pop pop\n"
      "  color_packet 0 get 0.299 mul\n"
      "  color_packet 1 get 0.587 mul add\n"
      "  color_packet 2 get 0.114 mul add\n"
      "  cvi\n"
      "  /gray_packet exch def\n"
      "  compression 0 eq\n"
      "  {\n"
      "    /number_pixels 1 def\n"
      "  }\n"
      "  {\n"
      "    currentfile byte readhexstring pop 0 get\n"
      "    /number_pixels exch 1 add def\n"
      "  } ifelse\n"
      "  0 1 number_pixels 1 sub\n"
      "  {\n"
      "    pixels exch gray_packet put\n"
      "  } for\n"
      "  pixels 0 number_pixels getinterval\n"
      "} bind def\n"
      "\n"
      "/GrayPseudoClassPacket\n"
      "{\n"
      "  %\n"
      "  % Get a PseudoClass packet;  convert to grayscale.\n"
      "  %\n"
      "  % Parameters:\n"
      "  %   index: index into the colormap.\n"
      "  %   length: number of pixels minus one of this color (optional).\n"
      "  %\n"
      "  currentfile byte readhexstring pop 0 get\n"
      "  /offset exch 3 mul def\n"
      "  /color_packet colormap offset 3 getinterval def\n"
      "  color_packet 0 get 0.299 mul\n"
      "  color_packet 1 get 0.587 mul add\n"
      "  color_packet 2 get 0.114 mul add\n"
      "  cvi\n"
      "  /gray_packet exch def\n"
      "  compression 0 eq\n"
      "  {\n"
      "    /number_pixels 1 def\n"
      "  }\n"
      "  {\n"
      "    currentfile byte readhexstring pop 0 get\n"
      "    /number_pixels exch 1 add def\n"
      "  } ifelse\n"
      "  0 1 number_pixels 1 sub\n"
      "  {\n"
      "    pixels exch gray_packet put\n"
      "  } for\n"
      "  pixels 0 number_pixels getinterval\n"
      "} bind def\n"
      "\n"
      "/PseudoClassPacket\n"
      "{\n"
      "  %\n"
      "  % Get a PseudoClass packet.\n"
      "  %\n"
      "  % Parameters:\n"
      "  %   index: index into the colormap.\n"
      "  %   length: number of pixels minus one of this color (optional).\n"
      "  %\n"
      "  currentfile byte readhexstring pop 0 get\n"
      "  /offset exch 3 mul def\n"
      "  /color_packet colormap offset 3 getinterval def\n"
      "  compression 0 eq\n"
      "  {\n"
      "    /number_pixels 3 def\n"
      "  }\n"
      "  {\n"
      "    currentfile byte readhexstring pop 0 get\n"
      "    /number_pixels exch 1 add 3 mul def\n"
      "  } ifelse\n"
      "  0 3 number_pixels 1 sub\n"
      "  {\n"
      "    pixels exch color_packet putinterval\n"
      "  } for\n"
      "  pixels 0 number_pixels getinterval\n"
      "} bind def\n"
      "\n"
      "/PseudoClassImage\n"
      "{\n"
      "  %\n"
      "  % Display a PseudoClass image.\n"
      "  %\n"
      "  % Parameters:\n"
      "  %   class: 0-PseudoClass or 1-Grayscale.\n"
      "  %\n"
      "  currentfile buffer readline pop\n"
      "  token pop /class exch def pop\n"
      "  class 0 gt\n"
      "  {\n"
      "    currentfile buffer readline pop\n"
      "    token pop /depth exch def pop\n"
      "    /grays columns 8 add depth sub depth mul 8 idiv string def\n"
      "    columns rows depth\n"
      "    [\n"
      "      columns 0 0\n"
      "      rows neg 0 rows\n"
      "    ]\n"
      "    { currentfile grays readhexstring pop } image\n"
      "  }\n"
      "  {\n"
      "    %\n"
      "    % Parameters:\n"
      "    %   colors: number of colors in the colormap.\n"
      "    %   colormap: red, green, blue color packets.\n"
      "    %\n"
      "    currentfile buffer readline pop\n"
      "    token pop /colors exch def pop\n"
      "    /colors colors 3 mul def\n"
      "    /colormap colors string def\n"
      "    currentfile colormap readhexstring pop pop\n"
      "    systemdict /colorimage known\n"
      "    {\n"
      "      columns rows 8\n"
      "      [\n"
      "        columns 0 0\n"
      "        rows neg 0 rows\n"
      "      ]\n"
      "      { PseudoClassPacket } false 3 colorimage\n"
      "    }\n"
      "    {\n"
      "      %\n"
      "      % No colorimage operator;  convert to grayscale.\n"
      "      %\n"
      "      columns rows 8\n"
      "      [\n"
      "        columns 0 0\n"
      "        rows neg 0 rows\n"
      "      ]\n"
      "      { GrayPseudoClassPacket } image\n"
      "    } ifelse\n"
      "  } ifelse\n"
      "} bind def\n"
      "\n"
      "/DisplayImage\n"
      "{\n"
      "  %\n"
      "  % Display a DirectClass or PseudoClass image.\n"
      "  %\n"
      "  % Parameters:\n"
      "  %   x & y translation.\n"
      "  %   x & y scale.\n"
      "  %   label pointsize.\n"
      "  %   image label.\n"
      "  %   image columns & rows.\n"
      "  %   class: 0-DirectClass or 1-PseudoClass.\n"
      "  %   compression: 0-none or 1-RunlengthEncoded.\n"
      "  %   hex color packets.\n"
      "  %\n"
      "  gsave\n"
      "  /buffer 512 string def\n"
      "  /byte 1 string def\n"
      "  /color_packet 3 string def\n"
      "  /pixels 768 string def\n"
      "\n"
      "  currentfile buffer readline pop\n"
      "  token pop /x exch def\n"
      "  token pop /y exch def pop\n"
      "  x y translate\n"
      "  currentfile buffer readline pop\n"
      "  token pop /x exch def\n"
      "  token pop /y exch def pop\n"
      "  currentfile buffer readline pop\n"
      "  token pop /pointsize exch def pop\n",
    PostscriptEpilog[] =
      "  x y scale\n"
      "  currentfile buffer readline pop\n"
      "  token pop /columns exch def\n"
      "  token pop /rows exch def pop\n"
      "  currentfile buffer readline pop\n"
      "  token pop /class exch def pop\n"
      "  currentfile buffer readline pop\n"
      "  token pop /compression exch def pop\n"
      "  class 0 gt { PseudoClassImage } { DirectClassImage } ifelse\n"
      "  grestore\n";

  char
    buffer[MaxTextExtent],
    date[MaxTextExtent],
    **labels,
    page_geometry[MaxTextExtent];

  CompressionType
    compression;

  const char
    *value;

  const StringInfo
    *profile;

  double
    pointsize;

  GeometryInfo
    geometry_info;

  IndexPacket
    index;

  MagickBooleanType
    status;

  MagickOffsetType
    scene;

  MagickStatusType
    flags;

  PixelPacket
    pixel;

  PointInfo
    delta,
    resolution,
    scale;

  RectangleInfo
    geometry,
    media_info,
    page_info;

  register const IndexPacket
    *indexes;

  register const PixelPacket
    *p;

  register ssize_t
    i,
    x;

  register unsigned char
    *q;

  SegmentInfo
    bounds;

  size_t
    bit,
    byte,
    imageListLength,
    length,
    page,
    text_size;

  ssize_t
    j,
    y;

  time_t
    timer;

  unsigned char
    pixels[2048];

  /*
    Open output image file.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickCoreSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  status=OpenBlob(image_info,image,WriteBinaryBlobMode,&image->exception);
  if (status == MagickFalse)
    return(status);
  (void) memset(&bounds,0,sizeof(bounds));
  compression=image->compression;
  if (image_info->compression != UndefinedCompression)
    compression=image_info->compression;
  page=1;
  scene=0;
  imageListLength=GetImageListLength(image);
  do
  {
    /*
      Scale relative to dots-per-inch.
    */
    if (image->colorspace != CMYKColorspace)
      (void) TransformImageColorspace(image,sRGBColorspace);
    delta.x=DefaultResolution;
    delta.y=DefaultResolution;
    resolution.x=image->x_resolution;
    resolution.y=image->y_resolution;
    if ((resolution.x == 0.0) || (resolution.y == 0.0))
      {
        flags=ParseGeometry(PSDensityGeometry,&geometry_info);
        resolution.x=geometry_info.rho;
        resolution.y=geometry_info.sigma;
        if ((flags & SigmaValue) == 0)
          resolution.y=resolution.x;
      }
    if (image_info->density != (char *) NULL)
      {
        flags=ParseGeometry(image_info->density,&geometry_info);
        resolution.x=geometry_info.rho;
        resolution.y=geometry_info.sigma;
        if ((flags & SigmaValue) == 0)
          resolution.y=resolution.x;
      }
    if (image->units == PixelsPerCentimeterResolution)
      {
        resolution.x=(double) ((size_t) (100.0*2.54*resolution.x+0.5)/100.0);
        resolution.y=(double) ((size_t) (100.0*2.54*resolution.y+0.5)/100.0);
      }
    SetGeometry(image,&geometry);
    (void) FormatLocaleString(page_geometry,MaxTextExtent,"%.20gx%.20g",
      (double) image->columns,(double) image->rows);
    if (image_info->page != (char *) NULL)
      (void) CopyMagickString(page_geometry,image_info->page,MaxTextExtent);
    else
      if ((image->page.width != 0) && (image->page.height != 0))
        (void) FormatLocaleString(page_geometry,MaxTextExtent,
          "%.20gx%.20g%+.20g%+.20g",(double) image->page.width,(double)
          image->page.height,(double) image->page.x,(double) image->page.y);
      else
        if ((image->gravity != UndefinedGravity) &&
            (LocaleCompare(image_info->magick,"PS") == 0))
          (void) CopyMagickString(page_geometry,PSPageGeometry,MaxTextExtent);
    (void) ConcatenateMagickString(page_geometry,">",MaxTextExtent);
    (void) ParseMetaGeometry(page_geometry,&geometry.x,&geometry.y,
      &geometry.width,&geometry.height);
    scale.x=PerceptibleReciprocal(resolution.x)*geometry.width*delta.x;
    geometry.width=(size_t) floor(scale.x+0.5);
    scale.y=PerceptibleReciprocal(resolution.y)*geometry.height*delta.y;
    geometry.height=(size_t) floor(scale.y+0.5);
    (void) ParseAbsoluteGeometry(page_geometry,&media_info);
    (void) ParseGravityGeometry(image,page_geometry,&page_info,
      &image->exception);
    if (image->gravity != UndefinedGravity)
      {
        geometry.x=(-page_info.x);
        geometry.y=(ssize_t) (media_info.height+page_info.y-image->rows);
      }
    pointsize=12.0;
    if (image_info->pointsize != 0.0)
      pointsize=image_info->pointsize;
    text_size=0;
    value=GetImageProperty(image,"label");
    if (value != (const char *) NULL)
      text_size=(size_t) (MultilineCensus(value)*pointsize+12);
    if (page == 1)
      {
        /*
          Output Postscript header.
        */
        if (LocaleCompare(image_info->magick,"PS") == 0)
          (void) CopyMagickString(buffer,"%!PS-Adobe-3.0\n",MaxTextExtent);
        else
          (void) CopyMagickString(buffer,"%!PS-Adobe-3.0 EPSF-3.0\n",
            MaxTextExtent);
        (void) WriteBlobString(image,buffer);
        (void) WriteBlobString(image,"%%Creator: (ImageMagick)\n");
        (void) FormatLocaleString(buffer,MaxTextExtent,"%%%%Title: (%s)\n",
          image->filename);
        (void) WriteBlobString(image,buffer);
        timer=GetMagickTime();
        (void) FormatMagickTime(timer,MaxTextExtent,date);
        (void) FormatLocaleString(buffer,MaxTextExtent,
          "%%%%CreationDate: (%s)\n",date);
        (void) WriteBlobString(image,buffer);
        bounds.x1=(double) geometry.x;
        bounds.y1=(double) geometry.y;
        bounds.x2=(double) geometry.x+scale.x;
        bounds.y2=(double) geometry.y+(geometry.height+text_size);
        if ((image_info->adjoin != MagickFalse) &&
            (GetNextImageInList(image) != (Image *) NULL))
          (void) CopyMagickString(buffer,"%%%%BoundingBox: (atend)\n",
            MaxTextExtent);
        else
          {
            (void) FormatLocaleString(buffer,MaxTextExtent,
              "%%%%BoundingBox: %.20g %.20g %.20g %.20g\n",ceil(bounds.x1-0.5),
              ceil(bounds.y1-0.5),floor(bounds.x2+0.5),floor(bounds.y2+0.5));
            (void) WriteBlobString(image,buffer);
            (void) FormatLocaleString(buffer,MaxTextExtent,
              "%%%%HiResBoundingBox: %g %g %g %g\n",bounds.x1,bounds.y1,
              bounds.x2,bounds.y2);
          }
        (void) WriteBlobString(image,buffer);
        profile=GetImageProfile(image,"8bim");
        if (profile != (StringInfo *) NULL)
          {
            /*
              Embed Photoshop profile.
            */
            (void) FormatLocaleString(buffer,MaxTextExtent,
              "%%BeginPhotoshop: %.20g",(double) GetStringInfoLength(profile));
            (void) WriteBlobString(image,buffer);
            for (i=0; i < (ssize_t) GetStringInfoLength(profile); i++)
            {
              if ((i % 32) == 0)
                (void) WriteBlobString(image,"\n% ");
              (void) FormatLocaleString(buffer,MaxTextExtent,"%02X",
                (unsigned int) (GetStringInfoDatum(profile)[i] & 0xff));
              (void) WriteBlobString(image,buffer);
            }
            (void) WriteBlobString(image,"\n%EndPhotoshop\n");
          }
        profile=GetImageProfile(image,"xmp");
DisableMSCWarning(4127)
        if (0 && (profile != (StringInfo *) NULL))
RestoreMSCWarning
          {
            /*
              Embed XML profile.
            */
            (void) WriteBlobString(image,"\n%begin_xml_code\n");
            (void) FormatLocaleString(buffer,MaxTextExtent,
               "\n%%begin_xml_packet: %.20g\n",(double)
               GetStringInfoLength(profile));
            (void) WriteBlobString(image,buffer);
            for (i=0; i < (ssize_t) GetStringInfoLength(profile); i++)
              (void) WriteBlobByte(image,GetStringInfoDatum(profile)[i]);
            (void) WriteBlobString(image,"\n%end_xml_packet\n%end_xml_code\n");
          }
        value=GetImageProperty(image,"label");
        if (value != (const char *) NULL)
          (void) WriteBlobString(image,
            "%%DocumentNeededResources: font Times-Roman\n");
        (void) WriteBlobString(image,"%%DocumentData: Clean7Bit\n");
        (void) WriteBlobString(image,"%%LanguageLevel: 1\n");
        if (LocaleCompare(image_info->magick,"PS") != 0)
          (void) WriteBlobString(image,"%%Pages: 1\n");
        else
          {
            /*
              Compute the number of pages.
            */
            (void) WriteBlobString(image,"%%Orientation: Portrait\n");
            (void) WriteBlobString(image,"%%PageOrder: Ascend\n");
            (void) FormatLocaleString(buffer,MaxTextExtent,"%%%%Pages: %.20g\n",
              image_info->adjoin != MagickFalse ? (double)
              GetImageListLength(image) : 1.0);
            (void) WriteBlobString(image,buffer);
          }
        (void) WriteBlobString(image,"%%EndComments\n");
        (void) WriteBlobString(image,"\n%%BeginDefaults\n");
        (void) WriteBlobString(image,"%%EndDefaults\n\n");
        if ((LocaleCompare(image_info->magick,"EPI") == 0) ||
            (LocaleCompare(image_info->magick,"EPSI") == 0) ||
            (LocaleCompare(image_info->magick,"EPT") == 0))
          {
            Image
              *preview_image;

            Quantum
              pixel;

            register ssize_t
              x;

            ssize_t
              y;

            /*
              Create preview image.
            */
            preview_image=CloneImage(image,0,0,MagickTrue,&image->exception);
            if (preview_image == (Image *) NULL)
              ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
            /*
              Dump image as bitmap.
            */
            (void) FormatLocaleString(buffer,MaxTextExtent,
              "%%%%BeginPreview: %.20g %.20g %.20g %.20g\n%%  ",(double)
              preview_image->columns,(double) preview_image->rows,1.0,
              (double) ((((preview_image->columns+7) >> 3)*preview_image->rows+
              35)/36));
            (void) WriteBlobString(image,buffer);
            q=pixels;
            for (y=0; y < (ssize_t) image->rows; y++)
            {
              p=GetVirtualPixels(preview_image,0,y,preview_image->columns,1,
                &preview_image->exception);
              if (p == (const PixelPacket *) NULL)
                break;
              indexes=GetVirtualIndexQueue(preview_image);
              bit=0;
              byte=0;
              for (x=0; x < (ssize_t) preview_image->columns; x++)
              {
                byte<<=1;
                pixel=ClampToQuantum(GetPixelLuma(image,p));
                if (pixel >= (Quantum) (QuantumRange/2))
                  byte|=0x01;
                bit++;
                if (bit == 8)
                  {
                    q=PopHexPixel(hex_digits,byte,q);
                    if ((q-pixels+8) >= 80)
                      {
                        *q++='\n';
                        (void) WriteBlob(image,q-pixels,pixels);
                        q=pixels;
                        (void) WriteBlobString(image,"%  ");
                      };
                    bit=0;
                    byte=0;
                  }
              }
              if (bit != 0)
                {
                  byte<<=(8-bit);
                  q=PopHexPixel(hex_digits,byte,q);
                  if ((q-pixels+8) >= 80)
                    {
                      *q++='\n';
                      (void) WriteBlob(image,q-pixels,pixels);
                      q=pixels;
                      (void) WriteBlobString(image,"%  ");
                    };
                };
            }
            if (q != pixels)
              {
                *q++='\n';
                (void) WriteBlob(image,q-pixels,pixels);
              }
            (void) WriteBlobString(image,"\n%%EndPreview\n");
            preview_image=DestroyImage(preview_image);
          }
        /*
          Output Postscript commands.
        */
        (void) WriteBlob(image,sizeof(PostscriptProlog)-1,
          (const unsigned char *) PostscriptProlog);
        value=GetImageProperty(image,"label");
        if (value != (const char *) NULL)
          {
            (void) WriteBlobString(image,
              "  /Times-Roman findfont pointsize scalefont setfont\n");
            for (j=(ssize_t) MultilineCensus(value)-1; j >= 0; j--)
            {
              (void) WriteBlobString(image,"  /label 512 string def\n");
              (void) WriteBlobString(image,
                "  currentfile label readline pop\n");
              (void) FormatLocaleString(buffer,MaxTextExtent,
                "  0 y %g add moveto label show pop\n",j*pointsize+12);
              (void) WriteBlobString(image,buffer);
            }
          }
        (void) WriteBlob(image,sizeof(PostscriptEpilog)-1,
          (const unsigned char *) PostscriptEpilog);
        if (LocaleCompare(image_info->magick,"PS") == 0)
          (void) WriteBlobString(image,"  showpage\n");
        (void) WriteBlobString(image,"} bind def\n");
        (void) WriteBlobString(image,"%%EndProlog\n");
      }
    (void) FormatLocaleString(buffer,MaxTextExtent,"%%%%Page:  1 %.20g\n",
      (double) (page++));
    (void) WriteBlobString(image,buffer);
    (void) FormatLocaleString(buffer,MaxTextExtent,
      "%%%%PageBoundingBox: %.20g %.20g %.20g %.20g\n",(double) geometry.x,
      (double) geometry.y,geometry.x+(double) geometry.width,geometry.y+(double)
      (geometry.height+text_size));
    (void) WriteBlobString(image,buffer);
    if ((double) geometry.x < bounds.x1)
      bounds.x1=(double) geometry.x;
    if ((double) geometry.y < bounds.y1)
      bounds.y1=(double) geometry.y;
    if ((double) (geometry.x+geometry.width-1) > bounds.x2)
      bounds.x2=(double) geometry.x+geometry.width-1;
    if ((double) (geometry.y+(geometry.height+text_size)-1) > bounds.y2)
      bounds.y2=(double) geometry.y+(geometry.height+text_size)-1;
    value=GetImageProperty(image,"label");
    if (value != (const char *) NULL)
      (void) WriteBlobString(image,"%%%%PageResources: font Times-Roman\n");
    if (LocaleCompare(image_info->magick,"PS") != 0)
      (void) WriteBlobString(image,"userdict begin\n");
    (void) WriteBlobString(image,"DisplayImage\n");
    /*
      Output image data.
    */
    (void) FormatLocaleString(buffer,MaxTextExtent,"%.20g %.20g\n%g %g\n%g\n",
      (double) geometry.x,(double) geometry.y,scale.x,scale.y,pointsize);
    (void) WriteBlobString(image,buffer);
    labels=(char **) NULL;
    value=GetImageProperty(image,"label");
    if (value != (const char *) NULL)
      labels=StringToList(value);
    if (labels != (char **) NULL)
      {
        for (i=0; labels[i] != (char *) NULL; i++)
        {
          (void) FormatLocaleString(buffer,MaxTextExtent,"%s \n",
            labels[i]);
          (void) WriteBlobString(image,buffer);
          labels[i]=DestroyString(labels[i]);
        }
        labels=(char **) RelinquishMagickMemory(labels);
      }
    (void) memset(&pixel,0,sizeof(pixel));
    pixel.opacity=(Quantum) TransparentOpacity;
    index=(IndexPacket) 0;
    x=0;
    if ((image_info->type != TrueColorType) &&
        (SetImageGray(image,&image->exception) != MagickFalse))
      {
        if (SetImageMonochrome(image,&image->exception) == MagickFalse)
          {
            Quantum
              pixel;

            /*
              Dump image as grayscale.
            */
            (void) FormatLocaleString(buffer,MaxTextExtent,
              "%.20g %.20g\n1\n1\n1\n8\n",(double) image->columns,(double)
              image->rows);
            (void) WriteBlobString(image,buffer);
            q=pixels;
            for (y=0; y < (ssize_t) image->rows; y++)
            {
              p=GetVirtualPixels(image,0,y,image->columns,1,
                &image->exception);
              if (p == (const PixelPacket *) NULL)
                break;
              for (x=0; x < (ssize_t) image->columns; x++)
              {
                pixel=(Quantum) ScaleQuantumToChar(ClampToQuantum(
                  GetPixelLuma(image,p)));
                q=PopHexPixel(hex_digits,(size_t) pixel,q);
                if ((q-pixels+8) >= 80)
                  {
                    *q++='\n';
                    (void) WriteBlob(image,q-pixels,pixels);
                    q=pixels;
                  }
                p++;
              }
              if (image->previous == (Image *) NULL)
                {
                  status=SetImageProgress(image,SaveImageTag,(MagickOffsetType)
                    y,image->rows);
                  if (status == MagickFalse)
                    break;
                }
            }
            if (q != pixels)
              {
                *q++='\n';
                (void) WriteBlob(image,q-pixels,pixels);
              }
          }
        else
          {
            ssize_t
              y;

            Quantum
              pixel;

            /*
              Dump image as bitmap.
            */
            (void) FormatLocaleString(buffer,MaxTextExtent,
              "%.20g %.20g\n1\n1\n1\n1\n",(double) image->columns,(double)
              image->rows);
            (void) WriteBlobString(image,buffer);
            q=pixels;
            for (y=0; y < (ssize_t) image->rows; y++)
            {
              p=GetVirtualPixels(image,0,y,image->columns,1,
                &image->exception);
              if (p == (const PixelPacket *) NULL)
                break;
              indexes=GetVirtualIndexQueue(image);
              bit=0;
              byte=0;
              for (x=0; x < (ssize_t) image->columns; x++)
              {
                byte<<=1;
                pixel=ClampToQuantum(GetPixelLuma(image,p));
                if (pixel >= (Quantum) (QuantumRange/2))
                  byte|=0x01;
                bit++;
                if (bit == 8)
                  {
                    q=PopHexPixel(hex_digits,byte,q);
                    if ((q-pixels+2) >= 80)
                      {
                        *q++='\n';
                        (void) WriteBlob(image,q-pixels,pixels);
                        q=pixels;
                      };
                    bit=0;
                    byte=0;
                  }
                p++;
              }
              if (bit != 0)
                {
                  byte<<=(8-bit);
                  q=PopHexPixel(hex_digits,byte,q);
                  if ((q-pixels+2) >= 80)
                    {
                      *q++='\n';
                      (void) WriteBlob(image,q-pixels,pixels);
                      q=pixels;
                    }
                };
              if (image->previous == (Image *) NULL)
                {
                  status=SetImageProgress(image,SaveImageTag,(MagickOffsetType)
                    y,image->rows);
                  if (status == MagickFalse)
                    break;
                }
            }
            if (q != pixels)
              {
                *q++='\n';
                (void) WriteBlob(image,q-pixels,pixels);
              }
          }
      }
    else
      if ((image->storage_class == DirectClass) ||
          (image->colors > 256) || (image->matte != MagickFalse))
        {
          /*
            Dump DirectClass image.
          */
          (void) FormatLocaleString(buffer,MaxTextExtent,"%.20g %.20g\n0\n%d\n",
            (double) image->columns,(double) image->rows,
            compression == RLECompression ? 1 : 0);
          (void) WriteBlobString(image,buffer);
          switch (compression)
          {
            case RLECompression:
            {
              /*
                Dump runlength-encoded DirectColor packets.
              */
              q=pixels;
              for (y=0; y < (ssize_t) image->rows; y++)
              {
                p=GetVirtualPixels(image,0,y,image->columns,1,
                  &image->exception);
                if (p == (const PixelPacket *) NULL)
                  break;
                pixel=(*p);
                length=255;
                for (x=0; x < (ssize_t) image->columns; x++)
                {
                  if ((GetPixelRed(p) == pixel.red) &&
                      (GetPixelGreen(p) == pixel.green) &&
                      (GetPixelBlue(p) == pixel.blue) &&
                      (GetPixelOpacity(p) == pixel.opacity) &&
                      (length < 255) && (x < (ssize_t) (image->columns-1)))
                    length++;
                  else
                    {
                      if (x > 0)
                        {
                          WriteRunlengthPacket(image,pixel,length,p);
                          if ((q-pixels+10) >= 80)
                            {
                              *q++='\n';
                              (void) WriteBlob(image,q-pixels,pixels);
                              q=pixels;
                            }
                        }
                      length=0;
                    }
                  pixel=(*p);
                  p++;
                }
                WriteRunlengthPacket(image,pixel,length,p);
                if ((q-pixels+10) >= 80)
                  {
                    *q++='\n';
                    (void) WriteBlob(image,q-pixels,pixels);
                    q=pixels;
                  }
                if (image->previous == (Image *) NULL)
                  {
                    status=SetImageProgress(image,SaveImageTag,
                      (MagickOffsetType) y,image->rows);
                    if (status == MagickFalse)
                      break;
                  }
              }
              if (q != pixels)
                {
                  *q++='\n';
                  (void) WriteBlob(image,q-pixels,pixels);
                }
              break;
            }
            case NoCompression:
            default:
            {
              /*
                Dump uncompressed DirectColor packets.
              */
              q=pixels;
              for (y=0; y < (ssize_t) image->rows; y++)
              {
                p=GetVirtualPixels(image,0,y,image->columns,1,
                  &image->exception);
                if (p == (const PixelPacket *) NULL)
                  break;
                for (x=0; x < (ssize_t) image->columns; x++)
                {
                  if ((image->matte != MagickFalse) &&
                      (GetPixelOpacity(p) == (Quantum) TransparentOpacity))
                    {
                      q=PopHexPixel(hex_digits,0xff,q);
                      q=PopHexPixel(hex_digits,0xff,q);
                      q=PopHexPixel(hex_digits,0xff,q);
                    }
                  else
                    {
                      q=PopHexPixel(hex_digits,ScaleQuantumToChar(
                        GetPixelRed(p)),q);
                      q=PopHexPixel(hex_digits,ScaleQuantumToChar(
                        GetPixelGreen(p)),q);
                      q=PopHexPixel(hex_digits,ScaleQuantumToChar(
                        GetPixelBlue(p)),q);
                    }
                  if ((q-pixels+6) >= 80)
                    {
                      *q++='\n';
                      (void) WriteBlob(image,q-pixels,pixels);
                      q=pixels;
                    }
                  p++;
                }
                if (image->previous == (Image *) NULL)
                  {
                    status=SetImageProgress(image,SaveImageTag,
                      (MagickOffsetType) y,image->rows);
                    if (status == MagickFalse)
                      break;
                  }
              }
              if (q != pixels)
                {
                  *q++='\n';
                  (void) WriteBlob(image,q-pixels,pixels);
                }
              break;
            }
          }
          (void) WriteBlobByte(image,'\n');
        }
      else
        {
          /*
            Dump PseudoClass image.
          */
          (void) FormatLocaleString(buffer,MaxTextExtent,
            "%.20g %.20g\n%d\n%d\n0\n",(double) image->columns,(double)
            image->rows,image->storage_class == PseudoClass ? 1 : 0,
            compression == RLECompression ? 1 : 0);
          (void) WriteBlobString(image,buffer);
          /*
            Dump number of colors and colormap.
          */
          (void) FormatLocaleString(buffer,MaxTextExtent,"%.20g\n",(double)
            image->colors);
          (void) WriteBlobString(image,buffer);
          for (i=0; i < (ssize_t) image->colors; i++)
          {
            (void) FormatLocaleString(buffer,MaxTextExtent,"%02X%02X%02X\n",
              ScaleQuantumToChar(image->colormap[i].red),
              ScaleQuantumToChar(image->colormap[i].green),
              ScaleQuantumToChar(image->colormap[i].blue));
            (void) WriteBlobString(image,buffer);
          }
          switch (compression)
          {
            case RLECompression:
            {
              /*
                Dump runlength-encoded PseudoColor packets.
              */
              q=pixels;
              for (y=0; y < (ssize_t) image->rows; y++)
              {
                p=GetVirtualPixels(image,0,y,image->columns,1,
                  &image->exception);
                if (p == (const PixelPacket *) NULL)
                  break;
                indexes=GetVirtualIndexQueue(image);
                index=GetPixelIndex(indexes);
                length=255;
                for (x=0; x < (ssize_t) image->columns; x++)
                {
                  if ((index == GetPixelIndex(indexes+x)) &&
                      (length < 255) && (x < ((ssize_t) image->columns-1)))
                    length++;
                  else
                    {
                      if (x > 0)
                        {
                          q=PopHexPixel(hex_digits,(size_t) index,q);
                          q=PopHexPixel(hex_digits,(size_t)
                            MagickMin(length,0xff),q);
                          i++;
                          if ((q-pixels+6) >= 80)
                            {
                              *q++='\n';
                              (void) WriteBlob(image,q-pixels,pixels);
                              q=pixels;
                            }
                        }
                      length=0;
                    }
                  index=GetPixelIndex(indexes+x);
                  pixel.red=GetPixelRed(p);
                  pixel.green=GetPixelGreen(p);
                  pixel.blue=GetPixelBlue(p);
                  pixel.opacity=GetPixelOpacity(p);
                  p++;
                }
                q=PopHexPixel(hex_digits,(size_t) index,q);
                q=PopHexPixel(hex_digits,(size_t) MagickMin(length,0xff),q);
                if ((q-pixels+6) >= 80)
                  {
                    *q++='\n';
                    (void) WriteBlob(image,q-pixels,pixels);
                    q=pixels;
                  }
                if (image->previous == (Image *) NULL)
                  {
                    status=SetImageProgress(image,SaveImageTag,
                      (MagickOffsetType) y,image->rows);
                    if (status == MagickFalse)
                      break;
                  }
              }
              if (q != pixels)
                {
                  *q++='\n';
                  (void) WriteBlob(image,q-pixels,pixels);
                }
              break;
            }
            case NoCompression:
            default:
            {
              /*
                Dump uncompressed PseudoColor packets.
              */
              q=pixels;
              for (y=0; y < (ssize_t) image->rows; y++)
              {
                p=GetVirtualPixels(image,0,y,image->columns,1,
                  &image->exception);
                if (p == (const PixelPacket *) NULL)
                  break;
                indexes=GetVirtualIndexQueue(image);
                for (x=0; x < (ssize_t) image->columns; x++)
                {
                  q=PopHexPixel(hex_digits,(size_t) GetPixelIndex(
                    indexes+x),q);
                  if ((q-pixels+4) >= 80)
                    {
                      *q++='\n';
                      (void) WriteBlob(image,q-pixels,pixels);
                      q=pixels;
                    }
                  p++;
                }
                if (image->previous == (Image *) NULL)
                  {
                    status=SetImageProgress(image,SaveImageTag,
                      (MagickOffsetType) y,image->rows);
                    if (status == MagickFalse)
                      break;
                  }
              }
              if (q != pixels)
                {
                  *q++='\n';
                  (void) WriteBlob(image,q-pixels,pixels);
                }
              break;
            }
          }
          (void) WriteBlobByte(image,'\n');
        }
    if (LocaleCompare(image_info->magick,"PS") != 0)
      (void) WriteBlobString(image,"end\n");
    (void) WriteBlobString(image,"%%PageTrailer\n");
    if (GetNextImageInList(image) == (Image *) NULL)
      break;
    image=SyncNextImageInList(image);
    status=SetImageProgress(image,SaveImagesTag,scene++,imageListLength);
    if (status == MagickFalse)
      break;
  } while (image_info->adjoin != MagickFalse);
  (void) WriteBlobString(image,"%%Trailer\n");
  if (page > 2)
    {
      (void) FormatLocaleString(buffer,MaxTextExtent,
        "%%%%BoundingBox: %.20g %.20g %.20g %.20g\n",ceil(bounds.x1-0.5),
        ceil(bounds.y1-0.5),floor(bounds.x2+0.5),floor(bounds.y2+0.5));
      (void) WriteBlobString(image,buffer);
      (void) FormatLocaleString(buffer,MaxTextExtent,
        "%%%%HiResBoundingBox: %g %g %g %g\n",bounds.x1,bounds.y1,
        bounds.x2,bounds.y2);
      (void) WriteBlobString(image,buffer);
    }
  (void) WriteBlobString(image,"%%EOF\n");
  (void) CloseBlob(image);
  return(MagickTrue);
}
