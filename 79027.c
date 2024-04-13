void Element::webkitRequestPointerLock()
{
    if (document()->page())
        document()->page()->pointerLockController()->requestPointerLock(this);
}
