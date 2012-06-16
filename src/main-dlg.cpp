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

#include "main-dlg.h"
#include "preferences-dlg.h"


QMainDialog::QMainDialog( const QString &file ) :
	m_ok( ":/icons/ok.png" ),
	m_cross( ":/icons/cross.png" )
{
	setupUi( this ); // this sets up GUI
	loadHashItems();
	
 	tableView->setModel( &m_model );
	tableView->setContextMenuPolicy( Qt::CustomContextMenu );
	pushButton->setIcon( m_cross );

	progressBar->setMinimum( 0 );
	progressBar->setMaximum( PROGRESS_SIZE );

	m_model.setColumnCount( 2 );

	connect( lineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( onCheck( const QString & ) ) );
	connect( buttonBox, SIGNAL( clicked( QAbstractButton * ) ), this, SLOT( onButtonClick( QAbstractButton * ) ) );
	connect( tableView, SIGNAL( customContextMenuRequested( const QPoint & ) ), this, SLOT( onShowContextMenu( const QPoint & ) ) );

	connect( actionOpenFile, SIGNAL( activated() ), this, SLOT( onOpen() ) );
	connect( actionQuit, SIGNAL( activated() ), this, SLOT( close() ) );

	connect( actionStart, SIGNAL( activated() ), this, SLOT( onStart() ) );
	connect( actionStop, SIGNAL( activated() ), this, SLOT( onStop() ) );
	connect( actionPreferences, SIGNAL( activated() ), this, SLOT( onPreferences() ) );

	connect( actionAbout, SIGNAL( activated() ), this, SLOT( onAbout() ) );
	connect( actionAboutQt, SIGNAL( activated() ), this, SLOT( onAboutQt() ) );

	actionOpenFile->setShortcut( tr( "Ctrl+O" ) );

	setFile( file );
	onStart();
}


QMainDialog::~QMainDialog( void )
{
	unloadHashItems();
}


void QMainDialog::loadHashItems( void )
{
	m_hashs.append( new QHashItem( "CRC32",  MHASH_CRC32,  true, true ) );
	m_hashs.append( new QHashItem( "CRC32B", MHASH_CRC32B, false ) );

	m_hashs.append( new QHashItem( "MD2", MHASH_MD2, false, true ) );
	m_hashs.append( new QHashItem( "MD4", MHASH_MD4, false ) );
	m_hashs.append( new QHashItem( "MD5", MHASH_MD5, true  ) );

	m_hashs.append( new QHashItem( "SHA1",   MHASH_SHA1,   true, true  ) );
	m_hashs.append( new QHashItem( "SHA224", MHASH_SHA224, false ) );
	m_hashs.append( new QHashItem( "SHA256", MHASH_SHA256, true  ) );
	m_hashs.append( new QHashItem( "SHA384", MHASH_SHA384, false ) );
	m_hashs.append( new QHashItem( "SHA512", MHASH_SHA512, false ) );

	m_hashs.append( new QHashItem( "HAVAL128", MHASH_HAVAL128, false, true ) );
	m_hashs.append( new QHashItem( "HAVAL160", MHASH_HAVAL160, false ) );
	m_hashs.append( new QHashItem( "HAVAL192", MHASH_HAVAL192, false ) );
	m_hashs.append( new QHashItem( "HAVAL224", MHASH_HAVAL224, false ) );
	m_hashs.append( new QHashItem( "HAVAL256", MHASH_HAVAL256, false ) );

	m_hashs.append( new QHashItem( "TIGER128", MHASH_TIGER128, false, true ) );
	m_hashs.append( new QHashItem( "TIGER160", MHASH_TIGER160, false ) );
	m_hashs.append( new QHashItem( "TIGER192", MHASH_TIGER192, false ) );

	m_hashs.append( new QHashItem( "RIPEMD128", MHASH_RIPEMD128, false, true ) );
	m_hashs.append( new QHashItem( "RIPEMD256", MHASH_RIPEMD256, false ) );
	m_hashs.append( new QHashItem( "RIPEMD320", MHASH_RIPEMD320, false ) );

	m_hashs.append( new QHashItem( "SNEFRU128", MHASH_SNEFRU128, false, true ) );
	m_hashs.append( new QHashItem( "SNEFRU256", MHASH_SNEFRU256, false ) );

	m_hashs.append( new QHashItem( "GOST",      MHASH_GOST,      false, true ) );
	m_hashs.append( new QHashItem( "RIPEMD160", MHASH_RIPEMD160, false ) );
	m_hashs.append( new QHashItem( "ADLER32",   MHASH_ADLER32,   false ) );
	m_hashs.append( new QHashItem( "WHIRLPOOL", MHASH_WHIRLPOOL, false ) );
}


void QMainDialog::unloadHashItems( void )
{
	foreach( QHashItem *item, m_hashs )
		delete item;
		
	m_hashs.clear();
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


void QMainDialog::onProgress( void )
{
	int prog = PROGRESS_SIZE;

	foreach( QCheckSum *item, m_calcs )
		prog = std::min( prog, item->progress() );

	progressBar->setValue( prog );
}


void QMainDialog::onUpdate( void )
{
	bool working = false;
	foreach( QCheckSum *item, m_calcs )
	{
		if( item->isRunning() )
		{
			working = true;
			break;
		}
	}

	actionStart->setEnabled( !working );
	actionStop->setEnabled( working );

	onCheck( lineEdit->text() );
}


void QMainDialog::onButtonClick( QAbstractButton *button )
{
	switch( buttonBox->standardButton( button ) )
	{
	case QDialogButtonBox::Close:
		close();
		break;

	case QDialogButtonBox::Open:
		onOpen();
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


void QMainDialog::onOpen( void )
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

	setFile( file );
	onStart();
}


void QMainDialog::onStart( void )
{
	clearModel();

	if( m_file.isEmpty() )
		return;

	foreach( QHashItem *item, m_hashs )
	{
		if( item->active() )
			m_calcs.append( new QCheckSum( this, item->id(), item->name(), m_file ) );
	}

	onUpdate();
}


void QMainDialog::onStop( void )
{
	foreach( QCheckSum *item, m_calcs )
	{
		if( item->isRunning() )
		{
			item->stop();
			item->wait();
		}
	}
	
	onUpdate();
}


void QMainDialog::onPreferences( void )
{
	QPreferencesDialog dialog( &m_hashs );
	
	if( dialog.exec() )
		onStart();
}


void QMainDialog::onAbout( void )
{
	QMessageBox::about
		(
			this,
			tr( ABOUT_APP ),
			tr
			(
				"<h4>" ABOUT_APP "</h4><br>"
				"This program is graphical user interface<br>"
				"for calculation and verification of the hash sum<br>"
				"with the help of the Mhash library.<br>"
				"Version 0.3a<br>"
				"<br>"
				"Developers:<br>"
				"Sergey Petrov <a href='mailto:" MAIL_PSE "?Subject=" APP_NAME "'>" MAIL_PSE "</a><br>"
				"Dmitriy Perlow<br>"
				"<br>"
				"Copyright (C) 2011-12 PetrovSE<br>"
				"<br>"
				"Mhash library: <a href='" WWW_MHASH "'>" WWW_MHASH "</a>"
			)
		);
}


void QMainDialog::onAboutQt( void )
{
	QMessageBox::aboutQt( this );
}


void QMainDialog::setFile( const QString &file )
{
	QString appName = APP_NAME;
	m_file = file;

	if( !m_file.isEmpty() )
	{
		QFileInfo info( m_file );
		appName += ": " + info.fileName();
		m_path   = info.absolutePath();
	}

	setWindowTitle( appName );
}


void QMainDialog::clearModel( void )
{
	onStop();

	foreach( QCheckSum *item, m_calcs )
		delete item;

	m_calcs.clear();
	m_model.clear();

	onUpdate();
}


void QMainDialog::closeEvent( QCloseEvent *event )
{
	clearModel();
	QMainWindow::closeEvent( event );
}
