#ifndef DOSEBASE_H
#define DOSEBASE_H

#include "slave_base.h"

class DoseBase: public SlaveBase
{
public:
    DoseBase();
public:

    static float compen_factor;
    static float ref_temp, ref_press;       //需要从文件中读取
    static float fwi[3], swi[3];
    static float dose_rate;
    static float x_lo_cf, x_hi_cf;

};

#endif // DOSEBASE_H
