#include "LexDict.h"




void bjfufst::NodeWord::AppendPhones(std::vector<std::string> phones)
{
	//cast vector to stack here.
	std::stack<std::string> stack_phones;
	for (auto &it_phone : phones)
	{
		stack_phones.push(it_phone);
	}

	if (stack_phones.empty())
	{
		return;
	}
	else
	{
		std::string phone = stack_phones.pop();
		this->strPhone = new char*[stack_phones.size()];
		strcpy_s(strPhone, phone.c_str());
		childPhones[childPhones.Insert(strPhone)].AppendPhones(phone);
	}
}



void bjfufst::NodePhone::AppendPhones(std::stack<std::string> & phones)
{
	if (phones.empty())
	{
		return;
	}
	else
	{
		std::string phone = phones.pop();
		this->strPhone = new char*[phone.size()];
		strcpy_s(strPhone, phone.c_str());
		childPhones[childPhones.Insert(strPhone)].AppendPhones(phone);
	}
}
