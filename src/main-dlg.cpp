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

#include <QtGui>
#include "main-dlg.h"
#include "checksum.h"

#define APP_NAME		"KCheckHash"


QMainDialog::QMainDialog( const QString &file ) :
	m_ok( ":/icons/ok.png" ),
	m_cross( ":/icons/cross.png" )
{
	setupUi( this ); // this sets up GUI

 	tableView->setModel( &m_model );
	tableView->setContextMenuPolicy( Qt::CustomContextMenu );
	pushButton->setIcon( m_cross );

	progressBar->setMinimum( 0 );
	progressBar->setMaximum( PROG_SIZE );

	m_model.setColumnCount( 2 );

	connect( this, SIGNAL( emitCheck( const QString & ) ), this, SLOT( onCheck( const QString & ) ) );
	connect( lineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( onCheck( const QString & ) ) );
	connect( buttonBox, SIGNAL( clicked( QAbstractButton * ) ), this, SLOT( onButtonClick( QAbstractButton * ) ) );
	connect( tableView, SIGNAL( customContextMenuRequested( const QPoint & ) ), this, SLOT( onShowContextMenu( const QPoint & ) ) );

	start( file );
}


void QMainDialog::onAdd( const QString &name, const QString &hash )
{
	QList<QStandardItem *> rows;
	QStandardItem *p_name = new QStandardItem;
	QStandardItem *p_hash = new QStandardItem;

	rows.append( p_name );
	rows.append( p_hash );

	p_name->setText( name );
	p_hash->setText( hash );

	m_model.appendRow( rows );
}


void QMainDialog::onCheck( const QString &hash )
{
	QString test = hash.trimmed();
	QString found;

	for( int n = 0 ; n < m_model.rowCount() ; n ++ )
	{
		QStandardItem *p_name = m_model.item( n, 0 );
		QStandardItem *p_hash = m_model.item( n, 1 );

		if
		(
			p_hash && p_name && !test.isEmpty()
			&&
			test.compare( p_hash->text(), Qt::CaseInsensitive ) == 0
		)
		{
			found = p_name->text();
			break;
		}
	}

	labelRes->setText( found );
	pushButton->setIcon( found.isEmpty() ? m_cross : m_ok );
}


void QMainDialog::onUpdate( void )
{
	int prog = PROG_SIZE;
	QCheckSum *item;

	foreach( item, m_calcs )
		prog = std::min( prog, item->progress() );

	progressBar->setValue( prog );
}


void QMainDialog::onFinished( void )
{
	emit emitCheck( lineEdit->text() );
}


void QMainDialog::onButtonClick( QAbstractButton *button )
{
	switch( buttonBox->standardButton( button ) )
	{
	case QDialogButtonBox::Close:
		close();
		break;

	case QDialogButtonBox::Open:
		open();
		break;

	default:
		break;
	}
}


void QMainDialog::onShowContextMenu( const QPoint &point )
{
	QClipboard *cb   = QApplication::clipboard();
	QModelIndex cell = tableView->indexAt( point );

	if( cb && cell.isValid() )
	{
		QStandardItem *item = m_model.item( cell.row(), cell.column() );

		if( item )
		{
			QMenu menu;
			QAction *actCopy = menu.addAction( "&Copy" );
			actCopy->setShortcut( QKeySequence::Copy );

			QAction *actRes = menu.exec( tableView->mapToGlobal( point ) );
			if( actRes == actCopy )
				cb->setText( item->text() );
		}
	}
}


void QMainDialog::closeEvent( QCloseEvent *event )
{
	stop();
	QMainWindow::closeEvent( event );
}


void QMainDialog::open( void )
{
	QString file = QFileDialog::getOpenFileName
		(
			this,
			"Open file",
			m_path,
			QString::null
		);

	if( file.isEmpty() )
		return;

	start( file );
}


void QMainDialog::start( const QString &file )
{
	stop();

	if( file.isEmpty() )
		return;

	QFileInfo info( file );
	setWindowTitle( APP_NAME ": " + info.fileName() );
	m_path = info.absolutePath();

	m_calcs.append( new QCheckSum( this,	MHASH_CRC32,	"CRC32",	file ) );
	m_calcs.append( new QCheckSum( this,	MHASH_MD5,		"MD5",		file ) );
	m_calcs.append( new QCheckSum( this,	MHASH_SHA1,		"SHA1",		file ) );
	m_calcs.append( new QCheckSum( this,	MHASH_SHA256,	"SHA256",	file ) );
}


void QMainDialog::stop( void )
{
	QCheckSum *item;
	foreach( item, m_calcs )
	{
		if( item->isRunning() )
		{
			item->stop();
			item->wait();
		}

		delete item;
	}

	m_calcs.clear();
	m_model.clear();

	setWindowTitle( APP_NAME );
}
