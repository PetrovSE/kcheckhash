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
#include "ui_main-dlg.h"

#define PROG_SIZE			256

class QCheckSum;
class QMainDialog : public QMainWindow, private Ui::mainDialog
{
	Q_OBJECT

public:
	QMainDialog( const QString &file = "" );

public slots:
	void onAdd( const QString &name, const QString &hash );
	void onCheck( const QString &hash );
	void onUpdate( void );
	void onFinished( void );
	void onButtonClick( QAbstractButton *button );
	void onShowContextMenu( const QPoint &point );

signals:
	void emitCheck( const QString &hash );

private:
	QStandardItemModel	m_model;
	QList <QCheckSum *>	m_calcs;

	QIcon				m_ok;
	QIcon				m_cross;
	QString				m_path;

	void closeEvent( QCloseEvent *event );

	void open( void );
	void start( const QString &file );
	void stop( void );
};

#endif // _MAIN_DLG_H_
