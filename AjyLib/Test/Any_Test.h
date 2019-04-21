#pragma once

void AnyTest()
{
	/************************************************************************/
	/* test1																*/
	/************************************************************************/
	int i = 1;
	Any any(i);
	bool isInt8 = any.IsType<int8>();
	bool isUInt8 = any.IsType<uint8>();
	bool isInt16 = any.IsType<int16>();
	bool isUInt16 = any.IsType<uint16>();
	bool isInt32 = any.IsType<int32>();
	bool isUInt32 = any.IsType<uint32>();
	bool isInt64 = any.IsType<int64>();
	bool isUInt64 = any.IsType<uint64>();
	bool isNull = any.IsNull();
	Console::WriteLine(any.TryGetValue<int32>());

	Any any1;
	bool isNull1 = any1.IsNull();
	any1 = any;
	isNull1 = any1.IsNull();
	isNull = any.IsNull();

	Any any2(std::move(any));
	bool isNull2 = any2.IsNull();
	isNull = any.IsNull();

	struct AnyTestHelper
	{
		int intData;
		bool boolData;
		double doubleData;
		std::string stdStrData;
		Ajy::String ajyStrData;
		AnyTestHelper(int i, bool b, double d, std::string stdstr, Ajy::String ajystr)
			:intData(i)
			, boolData(b)
			, doubleData(d)
			, stdStrData(std::move(stdstr))
			, ajyStrData(std::move(ajystr))
		{

		}
	};
	Any any3(AnyTestHelper(1, false, 0.123456789, std::string("helloworld"), Ajy::String::MakeString("cplusplus")));
	bool IsAnyTestHelper = any3.IsType<AnyTestHelper>();
	AnyTestHelper anyTestHelper = any3.TryGetValue<AnyTestHelper>();
	Console::WriteLine(anyTestHelper.intData, " ", anyTestHelper.boolData, " ", anyTestHelper.doubleData, " ", anyTestHelper.stdStrData, " ", anyTestHelper.ajyStrData);

	//any3 = any2;
	//any3 = (Any&&)any2;
	any3 = std::move(any2);
	Console::WriteLine(any3.TryGetValue<int>());
	Console::WriteLine("any2 is null: ", any2.IsNull());

	HashMap<int, Ajy::String> hashmap;
	for (int i = 0; i < 10; ++i)
	{
		hashmap.Add(i, String::MakeString(i));
	}
	Any any4(hashmap);
	bool isHashMap_int_ajystring = any4.IsType<CMHashMap<int, Ajy::String>>();
	bool isHashMap_int_stdstring = any4.IsType<CMHashMap<int, std::string>>();
	HashMap<int, Ajy::String> hashmap1 = any4.TryGetValue<HashMap<int, Ajy::String>>();
	for (int i = 0; i < hashmap1.Data().DataLen(); ++i)
	{
		Console::WriteLine(hashmap1.Data()[i]->key, " ", hashmap1.Data()[i]->value);
	}

	Dictionary<int, int> dict;
	for (int i = 0; i < 10; ++i)
	{
		dict.Add(i, i);
	}
	Any any5(dict);
	bool isDict_int32_uint32 = any5.IsType<Dictionary<int32, uint32>>();
	bool isDict_int32_int32 = any5.IsType<Dictionary<int, int32>>();
	Dictionary<int, int> dict1 = any5.TryGetValue<Dictionary<int32, int>>();
	for (int i = 0; i < dict1.count; ++i)
	{
		if (dict1.entries[i].hashCode < 0) continue;
		Console::WriteLine(dict1.KeyIndexAt(i), " ", dict1.IndexAt(i));
	}

	Map<String, std::string> ajyMap;
	for (int i = 0; i < 10; ++i)
	{
		char buffer[5];
		sprintf(buffer, "%d", i);
		ajyMap.Insert(std::pair<String, std::string>(String::MakeString(i, i), std::string(buffer)));
	}
	Any any6(ajyMap);
	Map<String, std::string> ajyMap1 = any6.TryGetValue<Map<String, std::string>>();
	for (auto it = ajyMap1.Begin(); it != ajyMap1.End(); ++it)
	{
		Console::WriteLine(it->first, " ", it->second);
	}

	CMHashMap<int, int> hmap;
	for (int i = 0; i < 10; ++i)
	{
		hmap.Add(i, i);
	}
	Any any7(std::move(hmap));
	bool isHMap_int32_uint32 = any7.IsType<CMHashMap<int32, uint32>>();
	bool isHMap_int32_int32 = any7.IsType<CMHashMap<int, int32>>();
	CMHashMap<int, int> hmap1 = any7.TryGetValue<CMHashMap<int32, int>>();
	for (int i = 0; i < hmap1.count; ++i)
	{
		if (hmap1.items[i].prev == -2) continue;
		Console::WriteLine(hmap1.IndexAtKey(i), " ", hmap1.IndexAtValue(i));
	}

	CMHashSet<int> hset;
	for (int i = 0; i < 10; ++i)
	{
		hset.Add(i);
	}
	Any any8(hset);
	CMHashSet<int> haset1 = any8.TryGetValue<CMHashSet<int32>>();
	for (int i = 0; i < haset1.count; ++i)
	{
		if (haset1.items[i].prev == -2) continue;
		Console::WriteLine(haset1.IndexAtKey(i));
	}

	any3 = std::move(any7);
	CMHashMap<int, int> hmap2 = any3.TryGetValue<CMHashMap<int32, int>>();
	for (int i = 0; i < hmap2.count; ++i)
	{
		if (hmap2.items[i].prev == -2) continue;
		Console::WriteLine(hmap2.IndexAtKey(i), " ", hmap2.IndexAtValue(i));
	}
}
