void jswrap_graphics_setFontCustom(JsVar *parent, JsVar *bitmap, int firstChar, JsVar *width, int height) {
  JsGraphics gfx; if (!graphicsGetFromVar(&gfx, parent)) return;

  if (!jsvIsString(bitmap)) {
    jsExceptionHere(JSET_ERROR, "Font bitmap must be a String");
    return;
  }
  if (firstChar<0 || firstChar>255) {
    jsExceptionHere(JSET_ERROR, "First character out of range");
    return;
  }
  if (!jsvIsString(width) && !jsvIsInt(width)) {
    jsExceptionHere(JSET_ERROR, "Font width must be a String or an integer");
    return;
  }
  if (height<=0 || height>255) {
   jsExceptionHere(JSET_ERROR, "Invalid height");
   return;
 }
  jsvObjectSetChild(parent, JSGRAPHICS_CUSTOMFONT_BMP, bitmap);
  jsvObjectSetChild(parent, JSGRAPHICS_CUSTOMFONT_WIDTH, width);
  jsvObjectSetChildAndUnLock(parent, JSGRAPHICS_CUSTOMFONT_HEIGHT, jsvNewFromInteger(height));
  jsvObjectSetChildAndUnLock(parent, JSGRAPHICS_CUSTOMFONT_FIRSTCHAR, jsvNewFromInteger(firstChar));
  gfx.data.fontSize = JSGRAPHICS_FONTSIZE_CUSTOM;
  graphicsSetVar(&gfx);
}
