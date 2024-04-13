void jswrap_graphics_setPixel(JsVar *parent, int x, int y, JsVar *color) {
  JsGraphics gfx; if (!graphicsGetFromVar(&gfx, parent)) return;
  unsigned int col = gfx.data.fgColor;
  if (!jsvIsUndefined(color))
    col = (unsigned int)jsvGetInteger(color);
  graphicsSetPixel(&gfx, (short)x, (short)y, col);
  gfx.data.cursorX = (short)x;
  gfx.data.cursorY = (short)y;
  graphicsSetVar(&gfx); // gfx data changed because modified area
}
