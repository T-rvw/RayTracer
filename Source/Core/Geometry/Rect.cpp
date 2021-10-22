#include "Rect.h"

bool Rect::isHit(double /*w0*/, double w1, double w2) const
{
    return w1 >= 0.0 && w1 <= 1.0 &&
           w2 >= 0.0 && w2 <= 1.0;
}