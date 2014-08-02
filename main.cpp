#include <iostream>

#include "CardinalityAnalyzer.h"
#include <QTextStream>

int main(int argc, char *argv[]){
	QTextStream cout(stdout);
	if (argc != 2){
		cout << "You forgot the argument" << endl;
		return 1;
	}
	CardinalityAnalyzer analyzer;
	cout << "processing ..." << endl;
	QString filename(argv[1]);
	auto result = analyzer.process(filename);
	cout << "done!" << endl;
	analyzer.dump(result);
	analyzer.dumpText(result);
	return 0;
}

