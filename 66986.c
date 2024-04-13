void jswrap_graphics_scroll(JsVar *parent, int xdir, int ydir) {
  JsGraphics gfx; if (!graphicsGetFromVar(&gfx, parent)) return;
  graphicsScroll(&gfx, xdir, ydir);
  graphicsSetVar(&gfx);
}
