#include "cfloatcompare.h"
#include <QtGlobal>

CFloatCompare::CFloatCompare()
{
}

bool CFloatCompare::FloatEqual(float num1, float num2)
{
    if(qAbs(num1-num2) > 0.000001)
        return true;
    else
        return false;
}

bool CFloatCompare::FloatGreatEqualZero(float num)
{
    if(FloatEqual(num, 0) || num > 0)
        return true;
    else
        return false;
}
