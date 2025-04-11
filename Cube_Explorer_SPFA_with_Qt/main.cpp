#include "CubeExplorerWithQt.h"
#include "CubeRecognizer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	
	QApplication a(argc, argv);
	CubeExplorerWithQt w;

	iniHSVMap();
	iniRecMap();
	iniRecogVars();
	ofstream out;
	out.open("test.out");
	out << "hello world" << endl;
	out.flush();
	out.close();

	w.setWindowTitle(QStringLiteral("Rubik's Cube Explorer"));
	w.setWindowIcon(QIcon("./Data/icon.png"));
	w.show();
	return a.exec();

	return 0;
}
