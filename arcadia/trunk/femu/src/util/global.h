#pragma once
#include <QtCore>
#include <assert.h>
#include <inttypes.h>

#define NOT_IMPLEMENTED assert(false);

#ifdef MODE_DEBUG
#   define ASSERT(x) assert(x)
#else
#   define ASSERT(x)
#endif

#define UNUSED(x) (void)(x)

typedef qint8 TInt8;
typedef quint8 TUInt8;

typedef qint16 TInt16;
typedef quint16 TUInt16;

typedef qint32 TInt32;
typedef quint32 TUInt32;

typedef qint64 TInt64;
typedef quint64 TUInt64;

typedef TUInt32 TIndex;
typedef TUInt32 TSize;
