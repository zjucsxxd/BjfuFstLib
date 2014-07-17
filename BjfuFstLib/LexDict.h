#include "CompactSet.h"
#include <vector>
#include <stack>
#include <string>
namespace bjfufst{
	class NodeWord;
	class NodePhone;
	class LexDict;

	class LexDict
	{
	public:
		LexDict();
		~LexDict();

	private:
		NodeWord initNode;
	};

	LexDict::LexDict()
	{
	}

	LexDict::~LexDict()
	{
	}

	class NodeWord
	{
	public:
		NodeWord();
		~NodeWord();
		void AppendPhones(std::vector<std::string> phones);
	private:
		char * strWord;
		CompactSet<NodePhone> childPhones;
	};


	NodeWord::NodeWord()
	{
	}

	NodeWord::~NodeWord()
	{
		childPhones.~CompactSet();
		delete strWord;
	}

	class NodePhone
	{
	public:
		NodePhone();
		~NodePhone();
		void AppendPhones(std::stack<std::string> & phones);

	private:
		char * strPhone;
		CompactSet<NodePhone> childPhones;
	};

	NodePhone::NodePhone()
	{
	}

	NodePhone::~NodePhone()
	{
		childPhones.~CompactSet();
		delete strPhone;
	}

}