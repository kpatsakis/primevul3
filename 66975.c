void jswrap_graphics_drawString(JsVar *parent, JsVar *var, int x, int y) {
  JsGraphics gfx; if (!graphicsGetFromVar(&gfx, parent)) return;

  JsVar *customBitmap = 0, *customWidth = 0;
  int customHeight = 0, customFirstChar = 0;
  if (gfx.data.fontSize>0) {
    customHeight = gfx.data.fontSize;
  } else if (gfx.data.fontSize == JSGRAPHICS_FONTSIZE_4X6) {
    customHeight = 6;
  } else if (gfx.data.fontSize == JSGRAPHICS_FONTSIZE_CUSTOM) {
    customBitmap = jsvObjectGetChild(parent, JSGRAPHICS_CUSTOMFONT_BMP, 0);
    customWidth = jsvObjectGetChild(parent, JSGRAPHICS_CUSTOMFONT_WIDTH, 0);
    customHeight = (int)jsvGetIntegerAndUnLock(jsvObjectGetChild(parent, JSGRAPHICS_CUSTOMFONT_HEIGHT, 0));
    customFirstChar = (int)jsvGetIntegerAndUnLock(jsvObjectGetChild(parent, JSGRAPHICS_CUSTOMFONT_FIRSTCHAR, 0));
  }

  JsGraphicsFlags oldFlags = gfx.data.flags;
  if (gfx.data.fontRotate==1) {
    gfx.data.flags ^= JSGRAPHICSFLAGS_SWAP_XY | JSGRAPHICSFLAGS_INVERT_X;
    int t = gfx.data.width - (x+1);
    x = y;
    y = t;
  } else if (gfx.data.fontRotate==2) {
    gfx.data.flags ^= JSGRAPHICSFLAGS_INVERT_X | JSGRAPHICSFLAGS_INVERT_Y;
    x = gfx.data.width - (x+1);
    y = gfx.data.height - (y+1);
  } else if (gfx.data.fontRotate==3) {
    gfx.data.flags ^= JSGRAPHICSFLAGS_SWAP_XY | JSGRAPHICSFLAGS_INVERT_Y;
    int t = gfx.data.height - (y+1);
    y = x;
    x = t;
  }
  if (gfx.data.fontAlignX>=0)
    x -= jswrap_graphics_stringWidth(parent, var) * (gfx.data.fontAlignX+1)/2;
  if (gfx.data.fontAlignY>=0)
    y -= customHeight * (gfx.data.fontAlignX+1)/2;

  int maxX = (gfx.data.flags & JSGRAPHICSFLAGS_SWAP_XY) ? gfx.data.height : gfx.data.width;
  int maxY = (gfx.data.flags & JSGRAPHICSFLAGS_SWAP_XY) ? gfx.data.width : gfx.data.height;
  int startx = x;
  JsVar *str = jsvAsString(var, false);
  JsvStringIterator it;
  jsvStringIteratorNew(&it, str, 0);
  while (jsvStringIteratorHasChar(&it)) {
    char ch = jsvStringIteratorGetChar(&it);
    if (ch=='\n') {
      x = startx;
      y += customHeight;
      jsvStringIteratorNext(&it);
      continue;
    }
    if (gfx.data.fontSize>0) {
#ifndef SAVE_ON_FLASH
      int w = (int)graphicsVectorCharWidth(&gfx, gfx.data.fontSize, ch);
      if (x>-w && x<maxX  && y>-gfx.data.fontSize && y<maxY)
        graphicsFillVectorChar(&gfx, (short)x, (short)y, gfx.data.fontSize, ch);
      x+=w;
#endif
    } else if (gfx.data.fontSize == JSGRAPHICS_FONTSIZE_4X6) {
      if (x>-4 && x<maxX && y>-6 && y<maxY)
        graphicsDrawChar4x6(&gfx, (short)x, (short)y, ch);
      x+=4;
    } else if (gfx.data.fontSize == JSGRAPHICS_FONTSIZE_CUSTOM) {
      int width = 0, bmpOffset = 0;
      if (jsvIsString(customWidth)) {
        if (ch>=customFirstChar) {
          JsvStringIterator wit;
          jsvStringIteratorNew(&wit, customWidth, 0);
          while (jsvStringIteratorHasChar(&wit) && (int)jsvStringIteratorGetIndex(&wit)<(ch-customFirstChar)) {
            bmpOffset += (unsigned char)jsvStringIteratorGetChar(&wit);
            jsvStringIteratorNext(&wit);
          }
          width = (unsigned char)jsvStringIteratorGetChar(&wit);
          jsvStringIteratorFree(&wit);
        }
      } else {
        width = (int)jsvGetInteger(customWidth);
        bmpOffset = width*(ch-customFirstChar);
      }
      if (ch>=customFirstChar && (x>-width) && (x<maxX) && (y>-customHeight) && y<maxY) {
        bmpOffset *= customHeight;
        JsvStringIterator cit;
        jsvStringIteratorNew(&cit, customBitmap, (size_t)bmpOffset>>3);
        bmpOffset &= 7;
        int cx,cy;
        for (cx=0;cx<width;cx++) {
          for (cy=0;cy<customHeight;cy++) {
            if ((jsvStringIteratorGetChar(&cit)<<bmpOffset)&128)
              graphicsSetPixel(&gfx, (short)(cx+x), (short)(cy+y), gfx.data.fgColor);
            bmpOffset++;
            if (bmpOffset==8) {
              bmpOffset=0;
              jsvStringIteratorNext(&cit);
            }
          }
        }
        jsvStringIteratorFree(&cit);
      }
      x += width;
    }
    if (jspIsInterrupted()) break;
    jsvStringIteratorNext(&it);
  }
  jsvStringIteratorFree(&it);
  jsvUnLock3(str, customBitmap, customWidth);
  gfx.data.flags = oldFlags; // restore flags because of text rotation
  graphicsSetVar(&gfx); // gfx data changed because modified area
}
