
#include "pch.h"


#include "mconfig/mpassivecfg/MsvPassiveConfig.h"
#include "mconfig/common/MsvConfigKeyMapBase.h"
#include "mconfig/common/MsvConfigKey.h"
#include "mconfig/common/MsvDefaultValue.h"

MSV_DISABLE_ALL_WARNINGS

#include <fstream>

MSV_ENABLE_WARNINGS


using namespace ::testing;


const char* const TEST_CONFIG_PATH = "test_config.ini";

const char* const TEST_CONFIG_GROUP1 = "GROUP_1";
const char* const TEST_CONFIG_GROUP2 = "GROUP_2";

const char* const TEST_CONFIG_BOOLVAL = "bool_value";
const char* const TEST_CONFIG_DOUBLEVAL = "double_value";
const char* const TEST_CONFIG_INT64VAL = "int64_t_value";
const char* const TEST_CONFIG_STRINGVAL = "string_value";
const char* const TEST_CONFIG_UINT64VAL = "uint64_t_value";


class MsvPassiveConfigKeyMapTest:
	public MsvConfigKeyMapBase<IMsvConfigKey>
{
public:
	MsvErrorCode Initialize()
	{
		MSV_RETURN_FAILED(InsertConfigKey(ConfigId::MSV_TEST_BOOL_1, TEST_CONFIG_GROUP1, TEST_CONFIG_BOOLVAL, std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(false))));
		MSV_RETURN_FAILED(InsertConfigKey(ConfigId::MSV_TEST_BOOL_2, TEST_CONFIG_GROUP2, TEST_CONFIG_BOOLVAL, std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(true))));
		MSV_RETURN_FAILED(InsertConfigKey(ConfigId::MSV_TEST_DOUBLE_1, TEST_CONFIG_GROUP1, TEST_CONFIG_DOUBLEVAL, std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(0.0))));
		MSV_RETURN_FAILED(InsertConfigKey(ConfigId::MSV_TEST_DOUBLE_2, TEST_CONFIG_GROUP2, TEST_CONFIG_DOUBLEVAL, std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(1.1))));
		MSV_RETURN_FAILED(InsertConfigKey(ConfigId::MSV_TEST_INTEGER_1, TEST_CONFIG_GROUP1, TEST_CONFIG_INT64VAL, std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(0ll))));
		MSV_RETURN_FAILED(InsertConfigKey(ConfigId::MSV_TEST_INTEGER_2, TEST_CONFIG_GROUP2, TEST_CONFIG_INT64VAL, std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(1ll))));
		MSV_RETURN_FAILED(InsertConfigKey(ConfigId::MSV_TEST_STRING_1, TEST_CONFIG_GROUP1, TEST_CONFIG_STRINGVAL, std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue("zero"))));
		MSV_RETURN_FAILED(InsertConfigKey(ConfigId::MSV_TEST_STRING_2, TEST_CONFIG_GROUP2, TEST_CONFIG_STRINGVAL, std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue("one"))));
		MSV_RETURN_FAILED(InsertConfigKey(ConfigId::MSV_TEST_UNSIGNED_1, TEST_CONFIG_GROUP1, TEST_CONFIG_UINT64VAL, std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(0ull))));
		MSV_RETURN_FAILED(InsertConfigKey(ConfigId::MSV_TEST_UNSIGNED_2, TEST_CONFIG_GROUP2, TEST_CONFIG_UINT64VAL, std::shared_ptr<IMsvDefaultValue>(new (std::nothrow) MsvDefaultValue(1ull))));

		return MSV_SUCCESS;
	}

	MsvErrorCode InsertConfigKey(ConfigId cfgId, const char* group, const char* key, std::shared_ptr<IMsvDefaultValue> spDefaultValue)
	{
		if (!spDefaultValue)
		{
			return MSV_ALLOCATION_ERROR;
		}

		std::shared_ptr<IMsvConfigKey> spConfigKey(new (std::nothrow) MsvConfigKey(group, key, spDefaultValue));
		if (!spConfigKey)
		{
			return MSV_ALLOCATION_ERROR;
		}

		return InsertKeyData(static_cast<int32_t>(cfgId), spConfigKey);
	}

};


class MsvPassiveConfig_Integration:
	public::testing::Test
{
public:
	MsvPassiveConfig_Integration()
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

		m_spConfigKeyMap.reset(new (std::nothrow) MsvPassiveConfigKeyMapTest());
		EXPECT_NE(m_spConfigKeyMap, nullptr);
		EXPECT_TRUE(MSV_SUCCEEDED(m_spConfigKeyMap->Initialize()));

		//delete config ini file
		remove(TEST_CONFIG_PATH);

		m_spPassiveCfg.reset(new (std::nothrow) MsvPassiveConfig());
		EXPECT_NE(m_spPassiveCfg, nullptr);
	}

	virtual void TearDown()
	{
		m_spPassiveCfg.reset();
		m_spConfigKeyMap.reset();

		//delete config ini file
		remove(TEST_CONFIG_PATH);
	}

	void CreateConfigIniFile()
	{
		//delete config ini file (if already exists)
		remove(TEST_CONFIG_PATH);

		std::ofstream iniConfigFile(TEST_CONFIG_PATH, std::ofstream::out | std::ofstream::trunc);
		EXPECT_TRUE(iniConfigFile.good());

		iniConfigFile << "#GROUP 1" << std::endl;
		iniConfigFile << ";GROUP 1" << std::endl;
		iniConfigFile << "[" << TEST_CONFIG_GROUP1 << "]" << std::endl;
		iniConfigFile << TEST_CONFIG_BOOLVAL << "=" << true << std::endl;
		iniConfigFile << TEST_CONFIG_DOUBLEVAL << "=" << 10.0 << std::endl;
		iniConfigFile << TEST_CONFIG_INT64VAL << "=" << 10 << std::endl;
		iniConfigFile << TEST_CONFIG_STRINGVAL << "=" << "ten" << std::endl;
		iniConfigFile << TEST_CONFIG_UINT64VAL << "=" << 10 << std::endl;

		iniConfigFile.close();
	}

	void CreateConfigIniFile2()
	{
		//delete config ini file (if already exists)
		remove(TEST_CONFIG_PATH);

		std::ofstream iniConfigFile(TEST_CONFIG_PATH, std::ofstream::out | std::ofstream::trunc);
		EXPECT_TRUE(iniConfigFile.good());

		iniConfigFile << "#GROUP 1" << std::endl;
		iniConfigFile << ";GROUP 1" << std::endl;
		iniConfigFile << "[" << TEST_CONFIG_GROUP1 << "]" << std::endl;
		iniConfigFile << TEST_CONFIG_BOOLVAL << "=" << true << std::endl;
		iniConfigFile << TEST_CONFIG_DOUBLEVAL << "=" << 10.0 << std::endl;
		iniConfigFile << TEST_CONFIG_INT64VAL << "=" << 10 << std::endl;
		iniConfigFile << TEST_CONFIG_STRINGVAL << "=" << "ten" << std::endl;
		iniConfigFile << TEST_CONFIG_UINT64VAL << "=" << 10 << std::endl;

		iniConfigFile << "#GROUP 2" << std::endl;
		iniConfigFile << ";GROUP 2" << std::endl;
		iniConfigFile << "[" << TEST_CONFIG_GROUP2 << "]" << std::endl;
		iniConfigFile << TEST_CONFIG_BOOLVAL << "=" << false << std::endl;
		iniConfigFile << TEST_CONFIG_DOUBLEVAL << "=" << 11.1 << std::endl;
		iniConfigFile << TEST_CONFIG_INT64VAL << "=" << 11 << std::endl;
		iniConfigFile << TEST_CONFIG_STRINGVAL << "=" << "eleven" << std::endl;
		iniConfigFile << TEST_CONFIG_UINT64VAL << "=" << 11 << std::endl;

		iniConfigFile.close();
	}

	void CreateConfigIniFile3()
	{
		//delete config ini file (if already exists)
		remove(TEST_CONFIG_PATH);

		std::ofstream iniConfigFile(TEST_CONFIG_PATH, std::ofstream::out | std::ofstream::trunc);
		EXPECT_TRUE(iniConfigFile.good());

		iniConfigFile << "#GROUP 1" << std::endl;
		iniConfigFile << ";GROUP 1" << std::endl;
		iniConfigFile << "[" << TEST_CONFIG_GROUP1 << "]" << std::endl;
		iniConfigFile << TEST_CONFIG_BOOLVAL << "=" << false << std::endl;
		iniConfigFile << TEST_CONFIG_DOUBLEVAL << "=" << 11.1 << std::endl;
		iniConfigFile << TEST_CONFIG_INT64VAL << "=" << 11 << std::endl;
		iniConfigFile << TEST_CONFIG_STRINGVAL << "=" << "eleven" << std::endl;
		iniConfigFile << TEST_CONFIG_UINT64VAL << "=" << 11 << std::endl;

		iniConfigFile << "#GROUP 2" << std::endl;
		iniConfigFile << ";GROUP 2" << std::endl;
		iniConfigFile << "[" << TEST_CONFIG_GROUP2 << "]" << std::endl;
		iniConfigFile << TEST_CONFIG_BOOLVAL << "=" << true << std::endl;
		iniConfigFile << TEST_CONFIG_DOUBLEVAL << "=" << 10.0 << std::endl;
		iniConfigFile << TEST_CONFIG_INT64VAL << "=" << 10 << std::endl;
		iniConfigFile << TEST_CONFIG_STRINGVAL << "=" << "ten" << std::endl;
		iniConfigFile << TEST_CONFIG_UINT64VAL << "=" << 10 << std::endl;

		iniConfigFile.close();
	}

	//logger
	std::shared_ptr<IMsvLoggerProvider> m_spLoggerProvider;
	std::shared_ptr<MsvLogger> m_spLogger;

	//test functions anc classes
	std::shared_ptr<MsvPassiveConfigKeyMapTest> m_spConfigKeyMap;
	std::shared_ptr<IMsvPassiveConfig> m_spPassiveCfg;
};


TEST_F(MsvPassiveConfig_Integration, ItShouldFailedWhenIniFileDoesNotExists)
{
	EXPECT_EQ(m_spPassiveCfg->Initialize(m_spConfigKeyMap, TEST_CONFIG_PATH), MSV_PARSE_ERROR);
}

TEST_F(MsvPassiveConfig_Integration, ItShouldSuccededWhenAlreadyInitialized)
{
	CreateConfigIniFile();
	EXPECT_EQ(m_spPassiveCfg->Initialize(m_spConfigKeyMap, TEST_CONFIG_PATH), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->Initialize(m_spConfigKeyMap, TEST_CONFIG_PATH), MSV_ALREADY_INITIALIZED_INFO);
}

TEST_F(MsvPassiveConfig_Integration, ReloadShouldFailedWhenNotInitialized)
{
	EXPECT_EQ(m_spPassiveCfg->ReloadConfiguration(), MSV_NOT_INITIALIZED_ERROR);
}

TEST_F(MsvPassiveConfig_Integration, ItShouldNotReturnValidErrorDataWhenNotFailed)
{
	int32_t lineNumberWithError;
	int32_t cfgIdWithError;
	m_spPassiveCfg->ReadFailedData(lineNumberWithError, cfgIdWithError);

	EXPECT_EQ(lineNumberWithError, INT32_MIN);
	EXPECT_EQ(cfgIdWithError, INT32_MIN);
}

TEST_F(MsvPassiveConfig_Integration, GetShouldFailedWhenNotInitialized)
{
	bool testBool1;
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_1), testBool1), MSV_NOT_INITIALIZED_ERROR);

	double testDouble1;
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_1), testDouble1), MSV_NOT_INITIALIZED_ERROR);

	int64_t testInteger1;
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_1), testInteger1), MSV_NOT_INITIALIZED_ERROR);

	std::string testString1;
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_1), testString1), MSV_NOT_INITIALIZED_ERROR);

	uint64_t testUnsigned1;
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_1), testUnsigned1), MSV_NOT_INITIALIZED_ERROR);
}

TEST_F(MsvPassiveConfig_Integration, GetShouldFailedWhenNotCfgIdDoesNotExist)
{
	CreateConfigIniFile();
	EXPECT_EQ(m_spPassiveCfg->Initialize(m_spConfigKeyMap, TEST_CONFIG_PATH), MSV_SUCCESS);

	bool testBool1;
	EXPECT_EQ(m_spPassiveCfg->GetValue(1000, testBool1), MSV_NOT_FOUND_ERROR);

	double testDouble1;
	EXPECT_EQ(m_spPassiveCfg->GetValue(1000, testDouble1), MSV_NOT_FOUND_ERROR);
	
	int64_t testInteger1;
	EXPECT_EQ(m_spPassiveCfg->GetValue(1000, testInteger1), MSV_NOT_FOUND_ERROR);
	
	std::string testString1;
	EXPECT_EQ(m_spPassiveCfg->GetValue(1000, testString1), MSV_NOT_FOUND_ERROR);

	uint64_t testUnsigned1;
	EXPECT_EQ(m_spPassiveCfg->GetValue(1000, testUnsigned1), MSV_NOT_FOUND_ERROR);
}

TEST_F(MsvPassiveConfig_Integration, ItShouldReturnDefaultValueForMissingIds)
{
	CreateConfigIniFile();
	EXPECT_EQ(m_spPassiveCfg->Initialize(m_spConfigKeyMap, TEST_CONFIG_PATH), MSV_SUCCESS);

	//ini file has been created -> it should return default values for missing values
	bool testBool1, testBool2;
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_1), testBool1), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_2), testBool2), MSV_SUCCESS);

	double testDouble1, testDouble2;
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_1), testDouble1), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_2), testDouble2), MSV_SUCCESS);

	int64_t testInteger1, testInteger2;
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_1), testInteger1), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_2), testInteger2), MSV_SUCCESS);

	std::string testString1, testString2;
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_1), testString1), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_2), testString2), MSV_SUCCESS);

	uint64_t testUnsigned1, testUnsigned2;
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_1), testUnsigned1), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_2), testUnsigned2), MSV_SUCCESS);
	
	EXPECT_EQ(testBool1, true);
	EXPECT_EQ(testBool2, true);
	EXPECT_EQ(testDouble1, 10.0);
	EXPECT_EQ(testDouble2, 1.1);
	EXPECT_EQ(testInteger1, 10);
	EXPECT_EQ(testInteger2, 1);
	EXPECT_EQ(testString1, "ten");
	EXPECT_EQ(testString2, "one");
	EXPECT_EQ(testUnsigned1, 10);
	EXPECT_EQ(testUnsigned2, 1);
}

TEST_F(MsvPassiveConfig_Integration, ItShouldReloadValues)
{
	CreateConfigIniFile2();
	EXPECT_EQ(m_spPassiveCfg->Initialize(m_spConfigKeyMap, TEST_CONFIG_PATH), MSV_SUCCESS);

	//ini file has been created -> it should return values
	bool testBool1, testBool2;
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_1), testBool1), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_2), testBool2), MSV_SUCCESS);

	double testDouble1, testDouble2;
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_1), testDouble1), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_2), testDouble2), MSV_SUCCESS);

	int64_t testInteger1, testInteger2;
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_1), testInteger1), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_2), testInteger2), MSV_SUCCESS);

	std::string testString1, testString2;
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_1), testString1), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_2), testString2), MSV_SUCCESS);

	uint64_t testUnsigned1, testUnsigned2;
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_1), testUnsigned1), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_2), testUnsigned2), MSV_SUCCESS);

	EXPECT_EQ(testBool1, true);
	EXPECT_EQ(testBool2, false);
	EXPECT_EQ(testDouble1, 10.0);
	EXPECT_EQ(testDouble2, 11.1);
	EXPECT_EQ(testInteger1, 10);
	EXPECT_EQ(testInteger2, 11);
	EXPECT_EQ(testString1, "ten");
	EXPECT_EQ(testString2, "eleven");
	EXPECT_EQ(testUnsigned1, 10);
	EXPECT_EQ(testUnsigned2, 11);

	CreateConfigIniFile3();
	EXPECT_TRUE(MSV_SUCCEEDED(m_spPassiveCfg->ReloadConfiguration()));

	//ini file has been rewritten -> it should return new values
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_1), testBool1), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_BOOL_2), testBool2), MSV_SUCCESS);

	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_1), testDouble1), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_DOUBLE_2), testDouble2), MSV_SUCCESS);

	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_1), testInteger1), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_INTEGER_2), testInteger2), MSV_SUCCESS);

	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_1), testString1), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_STRING_2), testString2), MSV_SUCCESS);

	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_1), testUnsigned1), MSV_SUCCESS);
	EXPECT_EQ(m_spPassiveCfg->GetValue(static_cast<int32_t>(ConfigId::MSV_TEST_UNSIGNED_2), testUnsigned2), MSV_SUCCESS);

	EXPECT_EQ(testBool1, false);
	EXPECT_EQ(testBool2, true);
	EXPECT_EQ(testDouble1, 11.1);
	EXPECT_EQ(testDouble2, 10.0);
	EXPECT_EQ(testInteger1, 11);
	EXPECT_EQ(testInteger2, 10);
	EXPECT_EQ(testString1, "eleven");
	EXPECT_EQ(testString2, "ten");
	EXPECT_EQ(testUnsigned1, 11);
	EXPECT_EQ(testUnsigned2, 10);
}