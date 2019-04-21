#pragma once

namespace TraitsTest
{
	class Custom
	{
		int i;
	};
}

void Traits_Test()
{
	cout << std::boolalpha;

	cout << is_tuple<std::tuple<int, double, bool, std::string>>::value << endl;
	cout << is_pair<std::tuple<int, double, bool, std::string>>::value << endl;
	cout << is_basic_type<std::tuple<int, double, bool, std::string>>::value << endl;
	cout << is_cahash<Ajy::cache_affinity_hash<cahash_map_params_t<int, int, std::true_type,Ajy::Hasher<int>, std::equal_to<int>, std::allocator<std::pair<int const, int>>>>>::value << endl;
	cout << is_cahash<Ajy::cache_affinity_hash<cahash_set_params_t<int, std::true_type, Ajy::Hasher<int>, std::equal_to<int>, std::allocator<int>>>>::value << endl;
	cout << is_cahash<Ajy::cahash_map<int, int>>::value << endl;
	cout << is_cahash_map<Ajy::cahash_map<int, int>>::value << endl;
	cout << "=====================================================" << endl;

	cout << is_pair<std::pair<int, std::string>>::value << endl;
	cout << is_tuple<std::pair<int, std::string>>::value << endl;
	cout << is_basic_type<std::pair<int, std::string>>::value << endl;
	cout << "=====================================================" << endl;

	cout << is_container<std::vector<int>>::value << endl;
	cout << is_singlevalue_container<std::vector<int>>::value << endl;
	cout << is_map_container<std::vector<int>>::value << endl;
	cout << is_normal_class<std::vector<int>>::value << endl;
	cout << is_basic_type<std::vector<int>>::value << endl;
	cout << is_user_class<std::vector<int>>::value << endl;
	cout << is_container_adapter<std::vector<int>>::value << endl;
	cout << "=====================================================" << endl;
	
	cout << is_container<std::queue<int>>::value << endl;
	cout << is_singlevalue_container<std::queue<int>>::value << endl;
	cout << is_map_container<std::queue<int>>::value << endl;
	cout << is_normal_class<std::queue<int>>::value << endl;
	cout << is_basic_type<std::queue<int>>::value << endl;
	cout << is_user_class<std::queue<int>>::value << endl;
	cout << is_container_adapter<std::queue<int>>::value << endl;
	cout << "=====================================================" << endl;

	cout << is_container<std::map<int,int>>::value << endl;
	cout << is_singlevalue_container<std::map<int, int>>::value << endl;
	cout << is_map_container<std::map<int, int>>::value << endl;
	cout << is_normal_class<std::map<int, int>>::value << endl;
	cout << is_basic_type<std::map<int, int>>::value << endl;
	cout << is_user_class<std::map<int, int>>::value << endl;
	cout << is_container_adapter<std::map<int, int>>::value << endl;
	cout << "=====================================================" << endl;

	cout << is_container<TraitsTest::Custom>::value << endl;
	cout << is_singlevalue_container<TraitsTest::Custom>::value << endl;
	cout << is_map_container<TraitsTest::Custom>::value << endl;
	cout << is_normal_class<TraitsTest::Custom>::value << endl;
	cout << is_basic_type<TraitsTest::Custom>::value << endl;
	cout << is_user_class<TraitsTest::Custom>::value << endl;
	cout << is_container_adapter<TraitsTest::Custom>::value << endl;
	cout << "=====================================================" << endl;

}
