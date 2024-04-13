void RenderBlock::appendFloatingObjectToLastLine(FloatingObject* floatingObject)
{
    ASSERT(!floatingObject->originatingLine());
    floatingObject->setOriginatingLine(lastRootBox());
    lastRootBox()->appendFloat(floatingObject->renderer());
}
