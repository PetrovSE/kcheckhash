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

#ifndef _MAIN_DLG_H_
#define _MAIN_DLG_H_

#include <QtGui>
#include "hashitem.h"
#include "checksum.h"
#include "ui_main-dlg.h"


#define APP_NAME		"KCheckHash"
#define ABOUT_APP		"About " APP_NAME

#define MAIL_PSE		"petrovse@mail.ru"
#define WWW_MHASH		"http://mhash.sourceforge.net/"


#define KDE_CFG_PATH	".kde/share/config/"
#define KDE_CFG_FILE	"kcheckhashrc"


class QMainDialog : public QMainWindow, private Ui::mainDialog
{
	Q_OBJECT

public:
	QMainDialog( const QString &file = "" );
	~QMainDialog( void );

public slots:
	void onAdd( const QString &name, const QString &hash );
	void onCheck( const QString &hash );

	void onProgress( void );
	void onUpdate( void );

	void onButtonClick( QAbstractButton *button );
	void onShowContextMenu( const QPoint &point );

	void onOpen( void );
	void onStart( void );
	void onStop( void );
	void onPreferences( void );

	void onAbout( void );
	void onAboutQt( void );

private:
	QStandardItemModel	m_model;

	QList <QCheckSum *>	m_calcs;
	QList <QHashItem *>	m_hashs;

	QIcon		m_ok;
	QIcon		m_cross;

	QString		m_path;
	QString		m_file;

	QMutex		m_lock;

	void loadHashItems( void );
	void unloadHashItems( void );
	
	void loadConfig( void );
	void saveConfig( void );

	void start( void );
	void stop( void );
	void clear( void );

	void setFile( const QString &file );
	void closeEvent( QCloseEvent *event );
};

#endif // _MAIN_DLG_H_
