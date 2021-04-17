//拿template做的小玩具
//多綫程編程中,我們時常需要獲取一些scope以外的變量
//一般我們會使用類的static函數, 然後靜態生命該變量,再聲明一個函數Getinstance返回該變量的地址
//從lua中獲取的靈感
//雖然沒什麽卵用
#include <map>

using globalDataType = template<typename T> map<string, T*> ;

globalDataType* Get_G()
{
	static globalDataType _G;
	return &_G;
}
template <typename T>
T::DataType* GetInstanceP(string&& name)
{
	if (Get_G()->at(name))
	{
		return static_cast<T::DataType*>(*(_G)[name]);
	}
}
class Base
{
	typedef Base DataType;
};

class Instance : public Base
{
	typedef Instance DataType;
};

int main()
{
	Instance* i = new Instance;
	
	_G.emplace(make_pair<string, Base*>("instance", i));


	//use 
	auto* p = GetInstanceP(instance);
	p->Dosomething();
}