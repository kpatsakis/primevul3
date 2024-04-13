void GraphicsContext3D::paintToCanvas(const unsigned char* imagePixels, int imageWidth, int imageHeight, int canvasWidth, int canvasHeight, CGContextRef context)
{
    if (!imagePixels || imageWidth <= 0 || imageHeight <= 0 || canvasWidth <= 0 || canvasHeight <= 0 || !context)
        return;
    int rowBytes = imageWidth * 4;
    RetainPtr<CGDataProviderRef> dataProvider(AdoptCF, CGDataProviderCreateWithData(0, imagePixels, rowBytes * imageHeight, 0));
    RetainPtr<CGImageRef> cgImage(AdoptCF, CGImageCreate(imageWidth, imageHeight, 8, 32, rowBytes, deviceRGBColorSpaceRef(), kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host,
        dataProvider.get(), 0, false, kCGRenderingIntentDefault));
    CGRect rect = CGRectMake(0, 0, canvasWidth, canvasHeight);
    CGContextSaveGState(context);
    CGContextSetBlendMode(context, kCGBlendModeCopy);
    CGContextSetInterpolationQuality(context, kCGInterpolationNone);
    CGContextDrawImage(context, rect, cgImage.get());
    CGContextRestoreGState(context);
}
