#ifndef CARDINALITYANALYZER_H
#define CARDINALITYANALYZER_H

#include <QString>
#include <string>
#include <unordered_map>
#include <map>

class CardinalityAnalyzer{
public:

	typedef std::pair<unsigned int, unsigned int> Cardinality;
	typedef std::string Key;
	typedef std::pair<Key, unsigned int> AttributeCount;
	typedef std::map<Key, unsigned int> AttributesCount;
	typedef std::pair<Key, Cardinality> Attribute;
	typedef std::map<Key, Cardinality> Attributes;
	typedef std::pair<std::string, Attributes> SpecificResult;
	typedef std::map<std::string, Attributes> Result;

	CardinalityAnalyzer();
	virtual Result process(QString& filename);
	virtual void dump(Result& result);
	virtual void dumpText(Result &result);
};

#endif // CARDINALITYANALYZER_H
