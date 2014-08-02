#include "CardinalityAnalyzer.h"
#include <iostream>

#include <QXmlStreamReader>
#include <QFile>
#include <QTextStream>

CardinalityAnalyzer::CardinalityAnalyzer(){
}

CardinalityAnalyzer::Result CardinalityAnalyzer::process(QString &filename){

	QTextStream cout(stdout);

	cout << "Starting... filename: " << filename << endl;

	QFile file(filename);
	bool open = file.open(QIODevice::ReadOnly | QIODevice::Text);
	if (!open){
		cout << "Couldn't open file" << endl;
		return Result();
	}
	else {
		cout << "File opened OK" << endl;
	}

	QXmlStreamReader xml(&file);
	cout << "Encoding: " << xml.documentEncoding().toString() << endl;
	cout << "Reading..." << endl;


	Result result;
	std::string sourceType = "undefined";
	AttributesCount sample;
	bool firstOfThatType = false;

	unsigned int i = 0;
	while (!xml.atEnd() && !xml.hasError()){
		xml.readNext();
		if (xml.isStartElement()){
			QString name = xml.name().toString();


			if (name == "article" || name == "book" ||
					name == "mastersthesis" || name == "phdthesis" ||
					name == "www" || name == "inproceedings"
					|| name == "proceedings" || name == "incollection"){

				if(sourceType == "article" || sourceType == "book" ||
						sourceType == "mastersthesis" || sourceType == "phdthesis" ||
						sourceType == "www" || sourceType == "inproceedings"
						|| sourceType == "proceedings" || sourceType == "incollection"){
					Attributes& attributes = result.at(sourceType);

					for(auto pair : attributes){
						if(sample.count(pair.first) == 0){
							attributes[pair.first].first = 0;
						}
					}


					for(auto pair : sample){
						if(attributes.count(pair.first) == 0){
							attributes.insert(Attribute(pair.first, Cardinality(0,1)));
							if(firstOfThatType){
								attributes[pair.first].first = 1;
							}
						}

						if(sample[pair.first] > attributes[pair.first].second){
							attributes[pair.first].second = sample[pair.first];
						}
					}

					firstOfThatType = false;
				}



				cout << /*"#" << */i++ /*<< " : " << name*/ << endl;
				sourceType = name.toStdString();
				if(result.count(sourceType) == 0){
					result.insert(SpecificResult(sourceType, Attributes()));
					firstOfThatType = true;
				}
				sample.clear();
			}


			else if (name != "dblp"){
				//cout << name << " : " << xml.readElementText(QXmlStreamReader::IncludeChildElements) << endl;
				std::string attributeName = name.toStdString();
				if(sample.count(attributeName) == 0){
					sample.insert(AttributeCount(attributeName, 0));
				}
				++sample.at(attributeName);
			}
		}
	}
	if (xml.hasError()){
		cout << "XML error: " << xml.errorString() << endl;
	}
	else if (xml.atEnd()){
		cout << "Reached end, done" << endl;
	}

	return result;
}

void CardinalityAnalyzer::dump(CardinalityAnalyzer::Result &result){
	for(SpecificResult specificResult : result){
		std::cout << specificResult.first << std::endl;
		std::cout << "--" << std::endl;
		for(Attribute attribute : specificResult.second){
			std::cout << attribute.first << " " << "(" << attribute.second.first << "," << attribute.second.second << ")" << std::endl;
		}
		std::cout << std::endl;
	}
}

void CardinalityAnalyzer::dumpText(CardinalityAnalyzer::Result &result){
	for(SpecificResult specificResult : result){
		std::cout << specificResult.first << std::endl;
		std::cout << "--" << std::endl;
		for(Attribute attribute : specificResult.second){
			std::cout << attribute.first << " " << "(" << attribute.second.first << "," << ((attribute.second.second == 1)? "1" : "n") << ")" << std::endl;
		}
		std::cout << std::endl;
	}
}
