/**********************************************************
* ACC data types and definitions
*
* June 2014 CUI Initial Creation
*
*/
#ifndef ACC_TYPE_H
#define ACC_TYPE_H
#include <basetsd.h>

//#define UINT8 unsigned char
//#define UINT16 unsigned short
//#define INT16 short
//#define UINT32 unsigned int
//#define INT32 int

#ifdef STATUS
    //Duplicate definition of STATUS
#else
    typedef int STATUS;
#endif

#define SECOND 1

#define EXTERNALFUNCTION extern
#define INTERNALFUNCTION
#define ACC_OK 1
#define ACC_ERROR 0
#define ACC_YES 1
#define ACC_NO 0

#endif
