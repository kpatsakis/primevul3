void jswrap_graphics_setFontSizeX(JsVar *parent, int size, bool checkValid) {
  JsGraphics gfx; if (!graphicsGetFromVar(&gfx, parent)) return;

  if (checkValid) {
    if (size<1) size=1;
    if (size>1023) size=1023;
  }
  if (gfx.data.fontSize == JSGRAPHICS_FONTSIZE_CUSTOM) {
    jsvObjectRemoveChild(parent, JSGRAPHICS_CUSTOMFONT_BMP);
    jsvObjectRemoveChild(parent, JSGRAPHICS_CUSTOMFONT_WIDTH);
    jsvObjectRemoveChild(parent, JSGRAPHICS_CUSTOMFONT_HEIGHT);
    jsvObjectRemoveChild(parent, JSGRAPHICS_CUSTOMFONT_FIRSTCHAR);
  }
  gfx.data.fontSize = (short)size;
  graphicsSetVar(&gfx);
}
