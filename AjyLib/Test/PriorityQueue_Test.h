#pragma once

void PriorityQueue_Test()
{
	PriorityQueue<int> pq;
	pq.Push(68);
	pq.Push(31);
	pq.Push(65);
	pq.Push(21);
	pq.Push(24);
	pq.Push(32);
	pq.Push(26);
	pq.Push(19);
	pq.Push(16);
	pq.Push(13);
	while (!pq.Empty())
	{
		cout << pq.Top() << "   ";
		pq.Pop();
	}
	cout << endl;
}
