unsigned int lcdGetPixel_ArrayBuffer_flat(JsGraphics *gfx, short x, short y) {
  unsigned int col = 0;
  unsigned char *ptr = (unsigned char*)gfx->backendData;
  unsigned int idx = lcdGetPixelIndex_ArrayBuffer(gfx,x,y,1);
  ptr += idx>>3;
  if (gfx->data.bpp&7/*not a multiple of one byte*/) {
    idx = idx & 7;
    unsigned int mask = (unsigned int)(1<<gfx->data.bpp)-1;
    unsigned int existing = (unsigned int)*ptr;
    unsigned int bitIdx = (gfx->data.flags & JSGRAPHICSFLAGS_ARRAYBUFFER_MSB) ? 8-(idx+gfx->data.bpp) : idx;
    col = ((existing>>bitIdx)&mask);
  } else {
    int i;
    for (i=0;i<gfx->data.bpp;i+=8) {
      col |= ((unsigned int)*ptr) << i;
      ptr++;
    }
  }
  return col;
}
