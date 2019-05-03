

#ifndef MARSTECH_SQLITECALLBACK_MOCK_H
#define MARSTECH_SQLITECALLBACK_MOCK_H


#include "mconfig/msqlitewrapper/IMsvSQLiteCallback.h"

#include <gmock/gmock.h>


class MsvSQLiteCallback_Mock:
	public IMsvSQLiteCallback
{
public:
	MOCK_METHOD4(OnChange, void(int operationType, const char *databaseName, const char *tableName, int64_t rowId));
};


#endif // MARSTECH_SQLITECALLBACK_MOCK_H
