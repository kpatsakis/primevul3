void jswrap_graphics_init() {
#ifdef USE_LCD_FSMC
  JsVar *parent = jspNewObject("LCD", "Graphics");
  if (parent) {
    JsVar *parentObj = jsvSkipName(parent);
    JsGraphics gfx;
    graphicsStructInit(&gfx);
    gfx.data.type = JSGRAPHICSTYPE_FSMC;
    gfx.graphicsVar = parentObj;
    gfx.data.width = 320;
    gfx.data.height = 240;
    gfx.data.bpp = 16;
    lcdInit_FSMC(&gfx);
    lcdSetCallbacks_FSMC(&gfx);
    graphicsSplash(&gfx);
    graphicsSetVar(&gfx);
    jsvUnLock2(parentObj, parent);
  }
#endif
}
