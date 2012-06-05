#include <QApplication>
#include "main-dlg.h"


int main( int nargs, char *argv[] )
{
	QApplication app( nargs, argv );
	QMainDialog *dialog;

	if( nargs > 1 )
		dialog = new QMainDialog( QString::fromUtf8( argv[1] ) );
	else
		dialog = new QMainDialog();

	dialog->show();
	return app.exec();
}
