void jswrap_graphics_lineTo(JsVar *parent, int x, int y) {
  JsGraphics gfx; if (!graphicsGetFromVar(&gfx, parent)) return;
  graphicsDrawLine(&gfx, gfx.data.cursorX, gfx.data.cursorY, (short)x, (short)y);
  gfx.data.cursorX = (short)x;
  gfx.data.cursorY = (short)y;
  graphicsSetVar(&gfx);
}
