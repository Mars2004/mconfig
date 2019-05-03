
#include "pch.h"


#include "mconfig/mactivecfg/MsvActiveConfig.h"
#include "mconfig/common/MsvConfigKeyMapBase.h"
#include "mconfig/common/MsvDefaultValue.h"
#include "mconfig/Mocks/MsvActiveConfigCallback_Mock.h"

#include "merror/MsvErrorCodes.h"

MSV_DISABLE_ALL_WARNINGS

#include <stdio.h>
#include <fstream>

MSV_ENABLE_WARNINGS


using namespace ::testing;


const char* const TEST_CONFIG_PATH = "test_config.db";
const char* const TEST_CONFIG_GROUP = "MsvTestConfig";


class MsvConfigKeyMapTest:
	public MsvConfigKeyMapBase<IMsvDefaultValue>
{
public:
	MsvErrorCode Initialize()
	{
		MSV_RETURN_FAILED(InsertKeyData(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_1), std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(false))));
		MSV_RETURN_FAILED(InsertKeyData(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_2), std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(true))));
		MSV_RETURN_FAILED(InsertKeyData(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_1), std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(0.0))));
		MSV_RETURN_FAILED(InsertKeyData(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_2), std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(1.1))));
		MSV_RETURN_FAILED(InsertKeyData(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_1), std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(0ll))));
		MSV_RETURN_FAILED(InsertKeyData(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_2), std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(1ll))));
		MSV_RETURN_FAILED(InsertKeyData(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_1), std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue("0"))));
		MSV_RETURN_FAILED(InsertKeyData(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_2), std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue("1"))));
		MSV_RETURN_FAILED(InsertKeyData(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_1), std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(0ull))));
		MSV_RETURN_FAILED(InsertKeyData(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_2), std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(1ull))));

		return MSV_SUCCESS;
	}

};

class MsvActiveConfig_Integration:
	public::testing::Test
{
public:
	MsvActiveConfig_Integration()
	{

	}

	virtual void SetUp()
	{
#if defined(MSV_TEST_WITH_LOGGING) && MSV_TEST_WITH_LOGGING != 0
		m_spLoggerProvider.reset(new (std::nothrow) MsvLoggerProvider());
#else
		m_spLoggerProvider.reset(new (std::nothrow) MsvNullLoggerProvider());
#endif
		EXPECT_NE(m_spLoggerProvider, nullptr);
		m_spLogger = m_spLoggerProvider->GetLogger();
		EXPECT_NE(m_spLogger, nullptr);
		
		//delete config database (new database for each test)
		remove(TEST_CONFIG_PATH);

		m_spConfigKeyMap.reset(new (std::nothrow) MsvConfigKeyMapTest());
		EXPECT_TRUE(m_spConfigKeyMap != nullptr);
		EXPECT_TRUE(MSV_SUCCEEDED(m_spConfigKeyMap->Initialize()));

		m_spActiveCfg.reset(new (std::nothrow) MsvActiveConfig(m_spLogger));
		EXPECT_TRUE(m_spActiveCfg != nullptr);

		m_spActiveCfgCallback.reset(new (std::nothrow) MsvActiveConfigCallback_Mock());
		EXPECT_TRUE(m_spActiveCfgCallback != nullptr);
	}

	virtual void TearDown()
	{
		m_spActiveCfg.reset();
		m_spConfigKeyMap.reset();

		//delete config database (new database for each test)
		remove(TEST_CONFIG_PATH);
	}

	bool FileExists(const char* fileName)
	{
		std::ifstream checkedFile(fileName);
		return checkedFile.good();
	}

	//logger
	std::shared_ptr<IMsvLoggerProvider> m_spLoggerProvider;
	std::shared_ptr<MsvLogger> m_spLogger;

	//mocks
	std::shared_ptr<MsvActiveConfigCallback_Mock> m_spActiveCfgCallback;

	//test functions anc classes
	std::shared_ptr<MsvConfigKeyMapTest> m_spConfigKeyMap;
	std::shared_ptr<IMsvActiveConfig> m_spActiveCfg;
};


TEST_F(MsvActiveConfig_Integration, ItShouldCreateDatabaseAndStoreDefaultValues)
{
	EXPECT_EQ(m_spActiveCfg->Initialize(m_spConfigKeyMap, TEST_CONFIG_PATH, TEST_CONFIG_GROUP), MSV_SUCCESS);
	EXPECT_TRUE(m_spActiveCfg->Initialized());

	//check if database exists
	EXPECT_TRUE(FileExists(TEST_CONFIG_PATH));

	//database has been created -> it should return default values
	bool testBool1, testBool2;
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_1), testBool1), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_2), testBool2), MSV_SUCCESS);

	double testDouble1, testDouble2;
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_1), testDouble1), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_2), testDouble2), MSV_SUCCESS);

	int64_t testInteger1, testInteger2;
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_1), testInteger1), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_2), testInteger2), MSV_SUCCESS);

	std::string testString1, testString2;
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_1), testString1), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_2), testString2), MSV_SUCCESS);

	uint64_t testUnsigned1, testUnsigned2;
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_1), testUnsigned1), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_2), testUnsigned2), MSV_SUCCESS);

	EXPECT_EQ(testBool1, false);
	EXPECT_EQ(testBool2, true);
	EXPECT_EQ(testDouble1, 0.0);
	EXPECT_EQ(testDouble2, 1.1);
	EXPECT_EQ(testInteger1, 0);
	EXPECT_EQ(testInteger2, 1);
	EXPECT_EQ(testString1, "0");
	EXPECT_EQ(testString2, "1");
	EXPECT_EQ(testUnsigned1, 0);
	EXPECT_EQ(testUnsigned2, 1);

	EXPECT_EQ(m_spActiveCfg->Uninitialize(), MSV_SUCCESS);
	EXPECT_FALSE(m_spActiveCfg->Initialized());
}

TEST_F(MsvActiveConfig_Integration, ItShouldStoreNewValuesAndReturnThem)
{
	EXPECT_EQ(m_spActiveCfg->Initialize(m_spConfigKeyMap, TEST_CONFIG_PATH, TEST_CONFIG_GROUP), MSV_SUCCESS);

	//check if database exists
	EXPECT_TRUE(FileExists(TEST_CONFIG_PATH));

	//set new values to database
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_1), true), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_2), false), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_1), 10.0), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_2), 10.1), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_1), 10ll), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_2), 11ll), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_1), "10"), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_2), "11"), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_1), 10ull), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_2), 11ull), MSV_SUCCESS);

	//database has been created and new values has been set -> it should return new values
	bool testBool1, testBool2;
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_1), testBool1), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_2), testBool2), MSV_SUCCESS);

	double testDouble1, testDouble2;
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_1), testDouble1), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_2), testDouble2), MSV_SUCCESS);

	int64_t testInteger1, testInteger2;
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_1), testInteger1), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_2), testInteger2), MSV_SUCCESS);

	std::string testString1, testString2;
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_1), testString1), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_2), testString2), MSV_SUCCESS);

	uint64_t testUnsigned1, testUnsigned2;
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_1), testUnsigned1), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_2), testUnsigned2), MSV_SUCCESS);

	EXPECT_EQ(testBool1, true);
	EXPECT_EQ(testBool2, false);
	EXPECT_EQ(testDouble1, 10.0);
	EXPECT_EQ(testDouble2, 10.1);
	EXPECT_EQ(testInteger1, 10);
	EXPECT_EQ(testInteger2, 11);
	EXPECT_EQ(testString1, "10");
	EXPECT_EQ(testString2, "11");
	EXPECT_EQ(testUnsigned1, 10);
	EXPECT_EQ(testUnsigned2, 11);

	EXPECT_EQ(m_spActiveCfg->Uninitialize(), MSV_SUCCESS);
}

TEST_F(MsvActiveConfig_Integration, ItShouldSucceededWhenInitializeExistingDatabase)
{
	EXPECT_EQ(m_spActiveCfg->Initialize(m_spConfigKeyMap, TEST_CONFIG_PATH, TEST_CONFIG_GROUP), MSV_SUCCESS);

	//check if database exists
	EXPECT_TRUE(FileExists(TEST_CONFIG_PATH));

	//set new values to database
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_1), true), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_2), false), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_1), 10.0), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_2), 10.1), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_1), 10ll), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_2), 11ll), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_1), "10"), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_2), "11"), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_1), 10ull), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_2), 11ull), MSV_SUCCESS);

	//create next active config instance
	std::shared_ptr<MsvActiveConfig> spActiveCfg2(new (std::nothrow) MsvActiveConfig(m_spLogger));
	EXPECT_TRUE(spActiveCfg2 != nullptr);
	EXPECT_EQ(spActiveCfg2->Initialize(m_spConfigKeyMap, TEST_CONFIG_PATH, TEST_CONFIG_GROUP), MSV_SUCCESS);

	//check if new values are returned by new config instance
	bool testBool1, testBool2;
	EXPECT_EQ(spActiveCfg2->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_1), testBool1), MSV_SUCCESS);
	EXPECT_EQ(spActiveCfg2->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_2), testBool2), MSV_SUCCESS);

	double testDouble1, testDouble2;
	EXPECT_EQ(spActiveCfg2->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_1), testDouble1), MSV_SUCCESS);
	EXPECT_EQ(spActiveCfg2->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_2), testDouble2), MSV_SUCCESS);

	int64_t testInteger1, testInteger2;
	EXPECT_EQ(spActiveCfg2->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_1), testInteger1), MSV_SUCCESS);
	EXPECT_EQ(spActiveCfg2->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_2), testInteger2), MSV_SUCCESS);

	std::string testString1, testString2;
	EXPECT_EQ(spActiveCfg2->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_1), testString1), MSV_SUCCESS);
	EXPECT_EQ(spActiveCfg2->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_2), testString2), MSV_SUCCESS);

	uint64_t testUnsigned1, testUnsigned2;
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_1), testUnsigned1), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_2), testUnsigned2), MSV_SUCCESS);

	EXPECT_EQ(testBool1, true);
	EXPECT_EQ(testBool2, false);
	EXPECT_EQ(testDouble1, 10.0);
	EXPECT_EQ(testDouble2, 10.1);
	EXPECT_EQ(testInteger1, 10);
	EXPECT_EQ(testInteger2, 11);
	EXPECT_EQ(testString1, "10");
	EXPECT_EQ(testString2, "11");
	EXPECT_EQ(testUnsigned1, 10);
	EXPECT_EQ(testUnsigned2, 11);

	EXPECT_EQ(m_spActiveCfg->Uninitialize(), MSV_SUCCESS);
	EXPECT_EQ(spActiveCfg2->Uninitialize(), MSV_SUCCESS);
}

TEST_F(MsvActiveConfig_Integration, ItShouldExecuteCallbacksWithNewValues)
{
	EXPECT_EQ(m_spActiveCfg->Initialize(m_spConfigKeyMap, TEST_CONFIG_PATH, TEST_CONFIG_GROUP), MSV_SUCCESS);

	//check if database exists
	EXPECT_TRUE(FileExists(TEST_CONFIG_PATH));

	//register callback
	EXPECT_EQ(m_spActiveCfg->RegisterCallback(m_spActiveCfgCallback), MSV_SUCCESS);

	//set callback expectations
	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_1), Matcher<bool>(true)));
	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_2), Matcher<bool>(false)));

	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_1), Matcher<double>(10.0)));
	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_2), Matcher<double>(10.1)));

	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_1), Matcher<int64_t>(10ll)));
	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_2), Matcher<int64_t>(11ll)));

	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_1), Matcher<const char*>(StrEq("10"))));
	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_2), Matcher<const char*>(StrEq("11"))));

	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_1), Matcher<uint64_t>(10ull)));
	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_2), Matcher<uint64_t>(11ull)));

	//set new values to database
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_1), true), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_2), false), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_1), 10.0), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_2), 10.1), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_1), 10ll), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_2), 11ll), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_1), "10"), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_2), "11"), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_1), 10ull), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_2), 11ull), MSV_SUCCESS);

	//unregister callback
	EXPECT_EQ(m_spActiveCfg->UnregisterCallback(m_spActiveCfgCallback), MSV_SUCCESS);
	
	EXPECT_EQ(m_spActiveCfg->Uninitialize(), MSV_SUCCESS);
}

/*
//This test does not work because SQLite callbacks works fine just for one connection.
//It means it can work only in one process. Cross process update detection does not work.
//Probably look for another storage.

TEST_F(MsvActiveConfig_Integration, ItShouldExecuteCallbacksWithNewValuesOnOtherConfigInstances)
{
	EXPECT_EQ(m_spActiveCfg->Initialize(m_spConfigKeyMap, TEST_CONFIG_PATH, TEST_CONFIG_GROUP), MSV_SUCCESS);

	//check if database exists
	EXPECT_TRUE(FileExists(TEST_CONFIG_PATH));

	//create next active config instance
	std::shared_ptr<MsvActiveConfig> spActiveCfg2(new (std::nothrow) MsvActiveConfig(m_spLogger));
	EXPECT_TRUE(spActiveCfg2 != nullptr);
	EXPECT_EQ(spActiveCfg2->Initialize(m_spConfigKeyMap, TEST_CONFIG_PATH, TEST_CONFIG_GROUP), MSV_SUCCESS);

	//register callback
	EXPECT_EQ(spActiveCfg2->RegisterCallback(m_spActiveCfgCallback), MSV_SUCCESS);

	//set callback expectations
	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_1), Matcher<bool>(true)));
	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_2), Matcher<bool>(false)));

	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_1), Matcher<double>(10.0)));
	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_2), Matcher<double>(10.1)));

	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_1), Matcher<int64_t>(10ll)));
	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_2), Matcher<int64_t>(11ll)));

	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_1), Matcher<const char*>(StrEq("10"))));
	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_2), Matcher<const char*>(StrEq("11"))));

	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_1), Matcher<uint64_t>(10ull)));
	EXPECT_CALL(*m_spActiveCfgCallback, OnValueChanged(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_2), Matcher<uint64_t>(11ull)));

	//set new values to database
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_1), true), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_2), false), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_1), 10.0), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_2), 10.1), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_1), 10ll), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_2), 11ll), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_1), "10"), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_2), "11"), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_1), 10ull), MSV_SUCCESS);
	EXPECT_EQ(m_spActiveCfg->SetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_2), 11ull), MSV_SUCCESS);

	//unregister callback
	EXPECT_EQ(spActiveCfg2->UnregisterCallback(m_spActiveCfgCallback), MSV_SUCCESS);

	EXPECT_EQ(m_spActiveCfg->Uninitialize(), MSV_SUCCESS);
	EXPECT_EQ(spActiveCfg2->Uninitialize(), MSV_SUCCESS);
}*/
