void jswrap_graphics_setRotation(JsVar *parent, int rotation, bool reflect) {
  JsGraphics gfx; if (!graphicsGetFromVar(&gfx, parent)) return;

  gfx.data.flags &= (JsGraphicsFlags)~(JSGRAPHICSFLAGS_SWAP_XY | JSGRAPHICSFLAGS_INVERT_X | JSGRAPHICSFLAGS_INVERT_Y);
  switch (rotation) {
    case 0:
      break;
    case 1:
      gfx.data.flags |= JSGRAPHICSFLAGS_SWAP_XY | JSGRAPHICSFLAGS_INVERT_X;
      break;
    case 2:
      gfx.data.flags |= JSGRAPHICSFLAGS_INVERT_X | JSGRAPHICSFLAGS_INVERT_Y;
      break;
    case 3:
      gfx.data.flags |= JSGRAPHICSFLAGS_SWAP_XY | JSGRAPHICSFLAGS_INVERT_Y;
      break;
  }

  if (reflect) {
    if (gfx.data.flags & JSGRAPHICSFLAGS_SWAP_XY)
      gfx.data.flags ^= JSGRAPHICSFLAGS_INVERT_Y;
    else
      gfx.data.flags ^= JSGRAPHICSFLAGS_INVERT_X;
  }

  graphicsSetVar(&gfx);
}
