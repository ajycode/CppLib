#pragma once

void Tuple_Test()
{

	Ajy::Tuple<double, int, double, int, std::string, const char*> t(2.1, 0, 1.11, 6666, "helloworld z", "world");
	cout << get<0>(t) << get<1>(t) << get<2>(t) << get<3>(t) << get<4>(t) << get<5>(t) << endl;
	get<0>(t) = 100;
	get<1>(t) = 10;
	get<4>(t) = "gg world";
	cout << get<0>(t) << get<1>(t) << get<2>(t) << get<3>(t) << get<4>(t) << get<5>(t) << endl;
	
	Ajy::Tuple<int, int, double, int, std::string>  tc(0, 0, 111.1, 5555, "helloworld");		// const对象不会被移动
	cout << get<0>(tc) << get<1>(tc) << get<2>(tc) << get<3>(tc) << get<4>(tc) << endl;

	cout << get<1>(Ajy::Tuple<double, char>(3.1415926, 'c')) << endl;
	cout << get<2>(Make_Tuple(3.1415926, 'c',"cplusplus11/14")) << endl;

	Ajy::Tuple<double, int, double, int, std::string, const char*> t1(t);
	cout << get<0>(t) << get<1>(t) << get<2>(t) << get<3>(t) << get<4>(t) << get<5>(t) << endl;
	cout << get<0>(t1) << get<1>(t1) << get<2>(t1) << get<3>(t1) << get<4>(t1) << get<5>(t1) << endl;

	Ajy::Tuple<int, int, double, int, std::string> tc1(std::move(tc));		// const对象不会被移动
	cout << get<0>(tc) << get<1>(tc) << get<2>(tc) << get<3>(tc) << get<4>(tc) << endl;
	cout << get<0>(tc1) << get<1>(tc1) << get<2>(tc1) << get<3>(tc1) << Ajy::get<4>(tc1) << endl;

	std::tuple<int, int, double, int, std::string> const std_t(0, 0, 111.1, 5555, "java");		// const对象不会被移动
	cout << get<0>(std_t) << get<1>(std_t) << get<2>(std_t) << get<3>(std_t) << std::get<4>(std_t) << endl;

	std::tuple<int, int, double, int, std::string> std_t1(std::move(std_t));		// const对象不会被移动
	cout << get<0>(std_t) << get<1>(std_t) << get<2>(std_t) << get<3>(std_t) << std::get<4>(std_t) << endl;
	cout << get<0>(std_t1) << get<1>(std_t1) << get<2>(std_t1) << get<3>(std_t1) << std::get<4>(std_t1) << endl;
}
