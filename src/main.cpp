/*   Copyright (C) 2011-2012 PetrovSE
 *   <http://forum.ubuntu.ru/index.php?action=profile;u=12963>
 * 
 *   This file is part of kcheckhash.
 *
 *   kcheckhash is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   kcheckhash is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with kcheckhash.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include "main-dlg.h"

#define APP_MAJOR_VERSION		0
#define APP_MINOR_VERSION		4


int main( int nargs, char *argv[] )
{
	QApplication app( nargs, argv );
	QMainDialog *dialog;

	QString locale = QLocale::system().name();
	QString appLoc = QString( APP_NAME ).toLower() + "_" + locale;
	QString trPath = QLibraryInfo::location( QLibraryInfo::TranslationsPath );

	QTranslator translator;
	QTranslator qtTranslator;

	if( !translator.load( appLoc ) )
		translator.load( appLoc, trPath );

	qtTranslator.load( "qt_" + locale, trPath );

	QString version;
	version.sprintf( "%d.%d", APP_MAJOR_VERSION, APP_MINOR_VERSION );
	app.setApplicationVersion( version );

	app.installTranslator( &translator );
	app.installTranslator( &qtTranslator );

	if( nargs > 1 )
		dialog = new QMainDialog( QTextCodec::codecForLocale()->toUnicode( argv[1] ) );
	else
		dialog = new QMainDialog();

	dialog->show();
	return app.exec();
}
