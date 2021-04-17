//��template����С���
//��Q�̾�����,�҂��r����Ҫ�@ȡһЩscope�����׃��
//һ���҂���ʹ���static����, Ȼ���o�B����ԓ׃��,����һ������Getinstance����ԓ׃���ĵ�ַ
//��lua�Ы@ȡ���`��
//�mȻ�]ʲ������
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