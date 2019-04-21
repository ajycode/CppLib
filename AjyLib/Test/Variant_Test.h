#pragma once

void Variant_Test()
{
	Ajy::Variant<int, char, double, bool, std::string, Ajy::bpptree_map<int, Ajy::String>> var, var1;
	Console::WriteLine(var.Empty());
	//var = 0.1f;		// 异常操作
	var = 1;
	Console::WriteLine(var.Get<int>());
	//Console::WriteLine(var.Get<float>());		// 异常操作
	Console::WriteLine(var.Get<0>());
	//Console::WriteLine(var.Get<6>());		// 异常操作
	Console::WriteLine(var.Empty());
	var.Clear();
	Console::WriteLine(var.Empty());
	var = std::string("helloworld");
	Console::WriteLine(var.IsType<std::string>());
	Console::WriteLine(var.Get<std::string>());
	Console::WriteLine(var.Get<4>());
	Console::WriteLine("--------------------------------------------------------------------");
	var1 = var;
	Console::WriteLine(var1.Get<std::string>());
	Console::WriteLine("--------------------------------------------------------------------");
	Ajy::Variant<int, char, double, bool, std::string, Ajy::bpptree_map<int, Ajy::String>> var2(std::move(var1));
	Console::WriteLine(var2.Get<std::string>());
	var2.Clear();
	Console::WriteLine(var2.Empty());
	Console::WriteLine("--------------------------------------------------------------------");
	Ajy::bpptree_map<int, Ajy::String> bptmap;
	for (int i = 0; i < 10; ++i)
	{
		bptmap.emplace(i, Ajy::String::MakeFormatString("{0}{1}{0}", i, "**"));
	}
	bptmap.emplace_hint(bptmap.cbegin(), -1, Ajy::String::MakeFormatString("{0}{1}{0}", -1, "*"));
	bptmap.emplace_hint(bptmap.cend(), 10, Ajy::String::MakeFormatString("{0}{1}{0}", 10, "***"));
	var2 = bptmap;
	Ajy::bpptree_map<int, Ajy::String> bptmap1 = var2.Get<Ajy::bpptree_map<int, Ajy::String>>();
	for (auto it = bptmap1.begin(); it != bptmap1.end(); ++it)
	{
		Console::Write(it->first, ",", it->second, "   ");
	}
	Console::WriteLine();
	var2 = 0.123456;
	Console::WriteLine(var2.Get<2>());
	var2 = 'a';
	Console::WriteLine(var2.Get<1>());
	var2 = true;
	Console::WriteLine(var2.Get<bool>());
}
