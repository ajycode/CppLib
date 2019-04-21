#pragma once

template<size_t... N>
std::vector<size_t> makeIndexVector(IndexSequence<N...>)
{
	return{ N... };
}

void MakeIndexSequence_Test()
{
	auto o = MakeIndexSequence<10>::type();
	std::vector<size_t> sizets = makeIndexVector(o);
	for (size_t i = 0; i < sizets.size(); ++i)
	{
		Console::WriteLine(sizets[i]);
	}
}
