JsVarInt jswrap_graphics_stringWidth(JsVar *parent, JsVar *var) {
  JsGraphics gfx; if (!graphicsGetFromVar(&gfx, parent)) return 0;

  JsVar *customWidth = 0;
  int customFirstChar = 0;
  if (gfx.data.fontSize == JSGRAPHICS_FONTSIZE_CUSTOM) {
    customWidth = jsvObjectGetChild(parent, JSGRAPHICS_CUSTOMFONT_WIDTH, 0);
    customFirstChar = (int)jsvGetIntegerAndUnLock(jsvObjectGetChild(parent, JSGRAPHICS_CUSTOMFONT_FIRSTCHAR, 0));
  }

  JsVar *str = jsvAsString(var, false);
  JsvStringIterator it;
  jsvStringIteratorNew(&it, str, 0);
  int width = 0;
  while (jsvStringIteratorHasChar(&it)) {
    char ch = jsvStringIteratorGetChar(&it);
    if (gfx.data.fontSize>0) {
#ifndef SAVE_ON_FLASH
      width += (int)graphicsVectorCharWidth(&gfx, gfx.data.fontSize, ch);
#endif
    } else if (gfx.data.fontSize == JSGRAPHICS_FONTSIZE_4X6) {
      width += 4;
    } else if (gfx.data.fontSize == JSGRAPHICS_FONTSIZE_CUSTOM) {
      if (jsvIsString(customWidth)) {
        if (ch>=customFirstChar)
          width += (unsigned char)jsvGetCharInString(customWidth, (size_t)(ch-customFirstChar));
      } else
        width += (int)jsvGetInteger(customWidth);
    }
    jsvStringIteratorNext(&it);
  }
  jsvStringIteratorFree(&it);
  jsvUnLock2(str, customWidth);
  return width;
}
