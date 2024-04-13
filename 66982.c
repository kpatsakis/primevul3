int jswrap_graphics_getWidthOrHeight(JsVar *parent, bool height) {
  JsGraphics gfx; if (!graphicsGetFromVar(&gfx, parent)) return 0;
  if (gfx.data.flags & JSGRAPHICSFLAGS_SWAP_XY)
    height=!height;
  return height ? gfx.data.height : gfx.data.width;
}
