

#ifndef MARSTECH_SQLITE_MOCK_H
#define MARSTECH_SQLITE_MOCK_H


#include "mconfig/msqlitewrapper/IMsvSQLite.h"

#include <gmock/gmock.h>


class MsvSQLite_Mock:
	public IMsvSQLite
{
public:
	MOCK_METHOD1(Initialize, MsvErrorCode(const char* dbPath));
	MOCK_METHOD0(Uninitialize, MsvErrorCode());
	MOCK_CONST_METHOD0(Initialized, bool());

	MOCK_METHOD3(CreateTableIfNotExists, MsvErrorCode(const char* tableName, const char* tableDef, const char* postCreateDefs));
	MOCK_METHOD2(Execute, MsvErrorCode(const char* query, MsvSQLiteResult& result));

	MOCK_METHOD1(RegisterCallback, MsvErrorCode(std::shared_ptr<IMsvSQLiteCallback> spCallback));
	MOCK_METHOD1(UnregisterCallback, MsvErrorCode(std::shared_ptr<IMsvSQLiteCallback> spCallback));
};


#endif // MARSTECH_SQLITE_MOCK_H
