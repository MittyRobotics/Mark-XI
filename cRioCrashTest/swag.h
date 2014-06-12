#ifndef SWAG_H
#define SWAG_H

#include "WPILib.h"

class swag
{
    public:
        swag();
        virtual ~swag();
        static swag* getPointer();
        void swagOut();
    protected:
    private:
        const float test;
};

#endif // SWAG_H
