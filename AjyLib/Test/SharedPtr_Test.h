#pragma once

class SharedPtrTestHelper
{
public:
	SharedPtrTestHelper()
	{
		cout << "constructor" << endl;
	}
	~SharedPtrTestHelper()
	{
		cout << "deconstructor" << endl;
	}
	int i = -1;
};

class Woman;
class Man
{
public:
	//std::weak_ptr<Woman> _wife;
	//std::shared_ptr<Woman> _wife;  
	//WeakPtr<Woman> _wife;
	SharedPtr<Woman> _wife;
public:
	/*void setWife(std::shared_ptr<Woman> woman)
	{
		_wife = woman;
	}*/

	void setWife(SharedPtr<Woman> woman)
	{
		_wife = woman;
	}

	Man(int _i) :i(_i)
	{
		cout << "create man\n";
	}

	~Man()
	{
		cout << "kill man\n";
	}
	int i = 1;
};
class Woman
{
public:
	//std::weak_ptr<Man> _husband;  
	//std::shared_ptr<Man> _husband;
	//WeakPtr<Man> _husband;
	SharedPtr<Man> _husband;
public:
	/*void setHusband(std::shared_ptr<Man> man)
	{
		_husband = man;
	}*/

	void setHusband(SharedPtr<Man> man)
	{
		_husband = man;
	}

	Woman(int _i) :i(_i)
	{
		cout << "create woman\n";
	}

	~Woman()
	{
		cout << "kill woman\n";
	}
	int i = 0;
};

void SharedPtr_Test()
{

	SharedPtr<SharedPtrTestHelper> shared = MakeSharedPtr<SharedPtrTestHelper>();
	WeakPtr<SharedPtrTestHelper> weak = shared;
	SharedPtr<SharedPtrTestHelper> shared1 = shared;
	SharedPtr<SharedPtrTestHelper> shared2 = shared;
	//SharedPtr<SharedPtrTestHelper> shared3 = shared;
	SharedPtr<SharedPtrTestHelper> shared3 = std::move(shared);
	//cout << shared->i << endl;
	cout << weak->i << endl;
	cout << shared1->i << endl;
	cout << shared2->i << endl;
	cout << shared3->i << endl;
	//cout << shared.st->copys << "   " << shared.st->weaks << endl;
	cout << weak.st->copys << "   " << weak.st->weaks << endl;
	cout << shared1.st->copys << "   " << shared1.st->weaks << endl;
	cout << shared2.st->copys << "   " << shared2.st->weaks << endl;
	cout << shared3.st->copys << "   " << shared3.st->weaks << endl;

	//std::shared_ptr<Man> m(new Man(11));
	//std::shared_ptr<Woman> w(new Woman(22));
	SharedPtr<Man> m(new SharedType<Man>(11));
	SharedPtr<Woman> w = MakeSharedPtr<Woman>(22);
	cout << m->i << endl;
	cout << w->i << endl;
	if (m.Ptr() != nullptr && w.Ptr() != nullptr)
	{
		cout << std::boolalpha;
		cout << (m->_wife.Ptr() == nullptr) << endl;
		cout << (w->_husband.Ptr() == nullptr) << endl;
		m->setWife(w);
		w->setHusband(m);
		cout << (m->_wife.Ptr() == nullptr) << endl;
		cout << (w->_husband.Ptr() == nullptr) << endl;
	}

}
