static bool layersUseImage(WrappedImagePtr image, const FillLayer* layers)
{
    for (const FillLayer* curLayer = layers; curLayer; curLayer = curLayer->next()) {
        if (curLayer->image() && image == curLayer->image()->data())
            return true;
    }

    return false;
}
