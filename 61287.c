static void SVGEndElement(void *context,const xmlChar *name)
{
  SVGInfo
    *svg_info;

  /*
    Called when the end of an element has been detected.
  */
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),
    "  SAX.endElement(%s)",name);
  svg_info=(SVGInfo *) context;
  if (strchr((char *) name,':') != (char *) NULL)
    {
      /*
        Skip over namespace.
      */
      for ( ; *name != ':'; name++) ;
      name++;
    }
  switch (*name)
  {
    case 'C':
    case 'c':
    {
      if (LocaleCompare((const char *) name,"circle") == 0)
        {
          (void) FormatLocaleFile(svg_info->file,"circle %g,%g %g,%g\n",
            svg_info->element.cx,svg_info->element.cy,svg_info->element.cx,
            svg_info->element.cy+svg_info->element.minor);
          (void) FormatLocaleFile(svg_info->file,"pop graphic-context\n");
          break;
        }
      if (LocaleCompare((const char *) name,"clipPath") == 0)
        {
          (void) FormatLocaleFile(svg_info->file,"pop clip-path\n");
          break;
        }
      break;
    }
    case 'D':
    case 'd':
    {
      if (LocaleCompare((const char *) name,"defs") == 0)
        {
          (void) FormatLocaleFile(svg_info->file,"pop defs\n");
          break;
        }
      if (LocaleCompare((const char *) name,"desc") == 0)
        {
          register char
            *p;

          if (*svg_info->text == '\0')
            break;
          (void) fputc('#',svg_info->file);
          for (p=svg_info->text; *p != '\0'; p++)
          {
            (void) fputc(*p,svg_info->file);
            if (*p == '\n')
              (void) fputc('#',svg_info->file);
          }
          (void) fputc('\n',svg_info->file);
          *svg_info->text='\0';
          break;
        }
      break;
    }
    case 'E':
    case 'e':
    {
      if (LocaleCompare((const char *) name,"ellipse") == 0)
        {
          double
            angle;

          angle=svg_info->element.angle;
          (void) FormatLocaleFile(svg_info->file,"ellipse %g,%g %g,%g 0,360\n",
            svg_info->element.cx,svg_info->element.cy,
            angle == 0.0 ? svg_info->element.major : svg_info->element.minor,
            angle == 0.0 ? svg_info->element.minor : svg_info->element.major);
          (void) FormatLocaleFile(svg_info->file,"pop graphic-context\n");
          break;
        }
      break;
    }
    case 'G':
    case 'g':
    {
      if (LocaleCompare((const char *) name,"g") == 0)
        {
          (void) FormatLocaleFile(svg_info->file,"pop graphic-context\n");
          break;
        }
      break;
    }
    case 'I':
    case 'i':
    {
      if (LocaleCompare((const char *) name,"image") == 0)
        {
          (void) FormatLocaleFile(svg_info->file,
            "image Over %g,%g %g,%g '%s'\n",svg_info->bounds.x,
            svg_info->bounds.y,svg_info->bounds.width,svg_info->bounds.height,
            svg_info->url);
          (void) FormatLocaleFile(svg_info->file,"pop graphic-context\n");
          break;
        }
      break;
    }
    case 'L':
    case 'l':
    {
      if (LocaleCompare((const char *) name,"line") == 0)
        {
          (void) FormatLocaleFile(svg_info->file,"line %g,%g %g,%g\n",
            svg_info->segment.x1,svg_info->segment.y1,svg_info->segment.x2,
            svg_info->segment.y2);
          (void) FormatLocaleFile(svg_info->file,"pop graphic-context\n");
          break;
        }
      if (LocaleCompare((const char *) name,"linearGradient") == 0)
        {
          (void) FormatLocaleFile(svg_info->file,"pop gradient\n");
          break;
        }
      break;
    }
    case 'P':
    case 'p':
    {
      if (LocaleCompare((const char *) name,"pattern") == 0)
        {
          (void) FormatLocaleFile(svg_info->file,"pop pattern\n");
          break;
        }
      if (LocaleCompare((const char *) name,"path") == 0)
        {
          (void) FormatLocaleFile(svg_info->file,"path '%s'\n",
            svg_info->vertices);
          (void) FormatLocaleFile(svg_info->file,"pop graphic-context\n");
          break;
        }
      if (LocaleCompare((const char *) name,"polygon") == 0)
        {
          (void) FormatLocaleFile(svg_info->file,"polygon %s\n",
            svg_info->vertices);
          (void) FormatLocaleFile(svg_info->file,"pop graphic-context\n");
          break;
        }
      if (LocaleCompare((const char *) name,"polyline") == 0)
        {
          (void) FormatLocaleFile(svg_info->file,"polyline %s\n",
            svg_info->vertices);
          (void) FormatLocaleFile(svg_info->file,"pop graphic-context\n");
          break;
        }
      break;
    }
    case 'R':
    case 'r':
    {
      if (LocaleCompare((const char *) name,"radialGradient") == 0)
        {
          (void) FormatLocaleFile(svg_info->file,"pop gradient\n");
          break;
        }
      if (LocaleCompare((const char *) name,"rect") == 0)
        {
          if ((svg_info->radius.x == 0.0) && (svg_info->radius.y == 0.0))
            {
              (void) FormatLocaleFile(svg_info->file,"rectangle %g,%g %g,%g\n",
                svg_info->bounds.x,svg_info->bounds.y,
                svg_info->bounds.x+svg_info->bounds.width,
                svg_info->bounds.y+svg_info->bounds.height);
              (void) FormatLocaleFile(svg_info->file,"pop graphic-context\n");
              break;
            }
          if (svg_info->radius.x == 0.0)
            svg_info->radius.x=svg_info->radius.y;
          if (svg_info->radius.y == 0.0)
            svg_info->radius.y=svg_info->radius.x;
          (void) FormatLocaleFile(svg_info->file,
            "roundRectangle %g,%g %g,%g %g,%g\n",
            svg_info->bounds.x,svg_info->bounds.y,svg_info->bounds.x+
            svg_info->bounds.width,svg_info->bounds.y+svg_info->bounds.height,
            svg_info->radius.x,svg_info->radius.y);
          svg_info->radius.x=0.0;
          svg_info->radius.y=0.0;
          (void) FormatLocaleFile(svg_info->file,"pop graphic-context\n");
          break;
        }
      break;
    }
    case 'S':
    case 's':
    {
      if (LocaleCompare((const char *) name,"stop") == 0)
        {
          (void) FormatLocaleFile(svg_info->file,"stop-color '%s' %s\n",
            svg_info->stop_color,svg_info->offset);
          break;
        }
      if (LocaleCompare((const char *) name,"svg") == 0)
        {
          (void) FormatLocaleFile(svg_info->file,"pop graphic-context\n");
          break;
        }
      break;
    }
    case 'T':
    case 't':
    {
      if (LocaleCompare((const char *) name,"text") == 0)
        {
          if (*svg_info->text != '\0')
            {
              char
                *text;

              text=EscapeString(svg_info->text,'\'');
              (void) FormatLocaleFile(svg_info->file,"text %g,%g '%s'\n",
                svg_info->bounds.x,svg_info->bounds.y,text);
              text=DestroyString(text);
              *svg_info->text='\0';
            }
          (void) FormatLocaleFile(svg_info->file,"pop graphic-context\n");
          break;
        }
      if (LocaleCompare((const char *) name,"tspan") == 0)
        {
          if (*svg_info->text != '\0')
            {
              DrawInfo
                *draw_info;

              TypeMetric
                metrics;

              char
                *text;

              text=EscapeString(svg_info->text,'\'');
              (void) FormatLocaleFile(svg_info->file,"text %g,%g '%s'\n",
                svg_info->bounds.x,svg_info->bounds.y,text);
              text=DestroyString(text);
              draw_info=CloneDrawInfo(svg_info->image_info,(DrawInfo *) NULL);
              draw_info->pointsize=svg_info->pointsize;
              draw_info->text=AcquireString(svg_info->text);
              (void) ConcatenateString(&draw_info->text," ");
              GetTypeMetrics(svg_info->image,draw_info,&metrics);
              svg_info->bounds.x+=metrics.width;
              draw_info=DestroyDrawInfo(draw_info);
              *svg_info->text='\0';
            }
          (void) FormatLocaleFile(svg_info->file,"pop graphic-context\n");
          break;
        }
      if (LocaleCompare((const char *) name,"title") == 0)
        {
          if (*svg_info->text == '\0')
            break;
          (void) CloneString(&svg_info->title,svg_info->text);
          *svg_info->text='\0';
          break;
        }
      break;
    }
    default:
      break;
  }
  *svg_info->text='\0';
  (void) ResetMagickMemory(&svg_info->element,0,sizeof(svg_info->element));
  (void) ResetMagickMemory(&svg_info->segment,0,sizeof(svg_info->segment));
  svg_info->n--;
}
