JsVar *jswrap_graphics_createSDL(int width, int height) {
  if (width<=0 || height<=0 || width>32767 || height>32767) {
    jsExceptionHere(JSET_ERROR, "Invalid Size");
    return 0;
  }

  JsVar *parent = jspNewObject(0, "Graphics");
  if (!parent) return 0; // low memory
  JsGraphics gfx;
  graphicsStructInit(&gfx);
  gfx.data.type = JSGRAPHICSTYPE_SDL;
  gfx.graphicsVar = parent;
  gfx.data.width = (unsigned short)width;
  gfx.data.height = (unsigned short)height;
  gfx.data.bpp = 32;
  lcdInit_SDL(&gfx);
  graphicsSetVar(&gfx);
  return parent;
}
