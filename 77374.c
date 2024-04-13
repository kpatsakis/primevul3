CGColorSpaceRef deviceRGBColorSpaceRef()
{
    static CGColorSpaceRef deviceSpace = CGColorSpaceCreateDeviceRGB();
    return deviceSpace;
}
