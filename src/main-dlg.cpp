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
	m_cross( ":/icons/cross.png" ),
	m_lock( QMutex::Recursive ),
	m_settings( QString( APP_NAME ).toLower(), QString( APP_NAME ).toLower() )
{
	setupUi( this ); // this sets up GUI
	resizeWindow();

	loadHashItems();
	loadConfig();
	
 	tableView->setModel( &m_model );
	tableView->setContextMenuPolicy( Qt::CustomContextMenu );
	pushButton->setIcon( m_cross );

	progressBar->setMinimum( 0 );
	progressBar->setMaximum( PROGRESS_SIZE );

	m_model.setColumnCount( 2 );

	buttonBox->addButton( &m_start, QDialogButtonBox::ResetRole );
	buttonBox->addButton( &m_stop, QDialogButtonBox::ResetRole );

	m_start.setText( tr( "Start" ) );
	m_stop.setText( tr( "Stop" ) );

	connectSignals();
	setAppearance();

	setFile( file );
	onStart( true, false );
}


QMainDialog::~QMainDialog( void )
{
	unloadHashItems();
}


void QMainDialog::connectSignals( void )
{
	connect( lineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( onCheck( const QString & ) ) );
	connect( buttonBox, SIGNAL( clicked( QAbstractButton * ) ), this, SLOT( onButtonClick( QAbstractButton * ) ) );
	connect( tableView, SIGNAL( customContextMenuRequested( const QPoint & ) ), this, SLOT( onShowContextMenu( const QPoint & ) ) );

	connect( &m_start, SIGNAL( clicked() ), this, SLOT( onStart() ) );
	connect( &m_stop, SIGNAL( clicked() ), this, SLOT( onStop() ) );

	connect( actionOpenFile, SIGNAL( triggered() ), this, SLOT( onOpen() ) );
	connect( actionQuit, SIGNAL( triggered() ), this, SLOT( close() ) );

	connect( actionStart, SIGNAL( triggered() ), this, SLOT( onStart() ) );
	connect( actionStop, SIGNAL( triggered() ), this, SLOT( onStop() ) );
	connect( actionPreferences, SIGNAL( triggered() ), this, SLOT( onPreferences() ) );

	connect( actionAbout, SIGNAL( triggered() ), this, SLOT( onAbout() ) );
	connect( actionAboutQt, SIGNAL( triggered() ), this, SLOT( onAboutQt() ) );
}


void QMainDialog::setAppearance( void )
{
	QStyle *mainStyle = style();

	m_start.setIcon( mainStyle->standardIcon( QStyle::SP_MediaPlay ) );
	m_stop.setIcon( mainStyle->standardIcon( QStyle::SP_MediaStop ) );

	actionOpenFile->setShortcut( QString( "Ctrl+O" ) );
	actionOpenFile->setIcon( mainStyle->standardIcon( QStyle::SP_DialogOpenButton ) );
	
	actionQuit->setIcon( mainStyle->standardIcon( QStyle::SP_DialogCloseButton ) );

	actionStart->setIcon( mainStyle->standardIcon( QStyle::SP_MediaPlay ) );
	actionStop->setIcon( mainStyle->standardIcon( QStyle::SP_MediaStop ) );

	actionPreferences->setIcon( QIcon::fromTheme( "configure" ) );

	actionAbout->setIcon( mainStyle->standardIcon( QStyle::SP_MessageBoxInformation ) );
	actionAboutQt->setIcon( QIcon( ":/icons/qt.png" ) );
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
	m_hashs.append( new QHashItem( "RIPEMD160", MHASH_RIPEMD160, false ) );
	m_hashs.append( new QHashItem( "RIPEMD256", MHASH_RIPEMD256, false ) );
	m_hashs.append( new QHashItem( "RIPEMD320", MHASH_RIPEMD320, false ) );

	m_hashs.append( new QHashItem( "SNEFRU128", MHASH_SNEFRU128, false, true ) );
	m_hashs.append( new QHashItem( "SNEFRU256", MHASH_SNEFRU256, false ) );

	m_hashs.append( new QHashItem( "ADLER32",   MHASH_ADLER32,   false, true ) );
	m_hashs.append( new QHashItem( "GOST",      MHASH_GOST,      false ) );
	m_hashs.append( new QHashItem( "WHIRLPOOL", MHASH_WHIRLPOOL, false ) );
}


void QMainDialog::unloadHashItems( void )
{
	foreach( QHashItem *item, m_hashs )
		delete item;
		
	m_hashs.clear();
}


void QMainDialog::loadConfig( void )
{
	foreach( QHashItem *item, m_hashs )
	{
		int val = m_settings.value( SEC_HASH + item->name(), -1 ).toInt();
		if( val < 0 )
			continue;
			
		item->setActive( val == 1 );
	}

	m_config.setAutoCalc( m_settings.value( KEY_AUTOCALC, true ).toBool() );
}


void QMainDialog::saveConfig( void )
{
	foreach( QHashItem *item, m_hashs )
		m_settings.setValue( SEC_HASH + item->name(), item->active() ? 1 : 0 );

	m_settings.setValue( KEY_AUTOCALC, m_config.autoCalc() );
	m_settings.sync();
}


void QMainDialog::onAdd( const QString &name, const QString &hash )
{
	QMutexLocker locker( &m_lock );
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
	QMutexLocker locker( &m_lock );
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
	QMutexLocker locker( &m_lock );
	int prog = PROGRESS_SIZE;

	foreach( QCheckSum *item, m_calcs )
		prog = std::min( prog, item->progress() );

	progressBar->setValue( prog );
}


void QMainDialog::onUpdate( void )
{
	QMutexLocker locker( &m_lock );
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

	m_start.setEnabled( !working );
	m_stop.setEnabled( working );

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
	onStart( true, false );
}


void QMainDialog::onStart( bool stop, bool force )
{
	if( stop )
	{
		clear();
		onUpdate();
	}

	if( ( !m_config.autoCalc() && !force ) || m_file.isEmpty() )
		return;
	
	start();
	onUpdate();
}


void QMainDialog::onStop( void )
{
	stop();
	onUpdate();
}


void QMainDialog::onPreferences( void )
{
	QPreferencesDialog dialog( m_hashs, m_config );
	int ret = dialog.exec();
	
	if( ret & QPrefCode::ChangeConfig )
		m_config = dialog.getConfig();

	if( ret & QPrefCode::NeedSave )
		saveConfig();
		
	if( ret & QPrefCode::ChangeHash )
		onStart( false, false );
}


void QMainDialog::onAbout( void )
{
	QString aboutText =
		H4S + tr( "About #APP_NAME" ) + H4T + BR
		+
		tr
		(
			"This program is graphical user interface<br>"
			"for calculation and verification of the hash sum<br>"
			"with the help of the Mhash library.<br>"
			"Version #VERSION"
		) + BR + BR
		+
		tr( "Developers:" ) + BR
		+
		tr( "Sergey Petrov" ) + " <a href='mailto:#MAIL_PSE?Subject=#APP_NAME'>#MAIL_PSE</a>" + BR
		+
		tr( "Dmitriy Perlow" ) + BR + BR
		+
		tr( "The source code:" ) + " <a href='#WWW_GITHUB'>#WWW_GITHUB</a>" + BR
		+
		tr( "Copyright (C) 2011-12 PetrovSE" ) + BR + BR
		+
		tr( "Mhash library:" ) + " <a href='#WWW_MHASH'>#WWW_MHASH</a>";

	aboutText.replace( "#APP_NAME", APP_NAME );
	aboutText.replace( "#VERSION", QApplication::applicationVersion() );

	aboutText.replace( "#MAIL_PSE", MAIL_PSE );

	aboutText.replace( "#WWW_GITHUB", WWW_GITHUB );
	aboutText.replace( "#WWW_MHASH", WWW_MHASH );

	QMessageBox::about
		(
			this,
			tr( "About" ),
			aboutText
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


void QMainDialog::start( void )
{
	clear();

	foreach( QHashItem *item, m_hashs )
	{
		if( item->active() )
			m_calcs.append( new QCheckSum( this, item->id(), item->name(), m_file ) );
	}
}


void QMainDialog::stop( void )
{
	foreach( QCheckSum *item, m_calcs )
	{
		if( item->isRunning() )
		{
			item->stop();
			item->wait();
		}
	}
}


void QMainDialog::clear( void )
{
	stop();

	foreach( QCheckSum *item, m_calcs )
		delete item;

	m_calcs.clear();
	m_model.clear();
}


void QMainDialog::resizeWindow( void )
{
	int width  = m_settings.value( KEY_WIDTH,  -1 ).toInt();
	int height = m_settings.value( KEY_HEIGHT, -1 ).toInt();
	
	if( width < 0 || height < 0 )
		return;

	resize( width, height );
}


void QMainDialog::closeEvent( QCloseEvent *event )
{
	m_settings.setValue( KEY_WIDTH,  width() ); 
	m_settings.setValue( KEY_HEIGHT, height() );
	m_settings.sync();

	clear();
	QMainWindow::closeEvent( event );
}
