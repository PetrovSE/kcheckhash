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

#include "checksum.h"


QCheckSum::QCheckSum( QMainWindow *parent, hashid id, const QString &name, const QString &file ) :
	m_id( id ),
	m_name( name ),
	m_file( file )
{
	m_stop		= false;
	m_progress	= 0;

	connect( this, SIGNAL( sigAdd( const QString &, const QString & ) ), parent, SLOT( onAdd( const QString &, const QString & ) ) );
	connect( this, SIGNAL( sigProgress( void ) ), parent, SLOT( onProgress( void ) ) );
	connect( this, SIGNAL( finished( void ) ), parent, SLOT( onUpdate( void ) ) );

	start();
}


void QCheckSum::run( void )
{
	quint8 *buff, *hash;
	MHASH td;

	while( 1 )
	{
		int len = mhash_get_block_size( m_id );

		buff = new quint8[BUFF_SIZE];
		hash = new quint8[len];

		if( !m_file.open( QIODevice::ReadOnly ) )
			break;

		td = mhash_init( m_id );
		if( td == MHASH_FAILED )
			break;

		double percent = (double)PROGRESS_SIZE / ( (double)m_file.size() + 0.1 );


		while( 1 )
		{
			int load = (int)m_file.read( (char *)buff, BUFF_SIZE );

			m_lock.lock();
			if( m_stop )
				load = -1;
			m_lock.unlock();

			if( load < 0 )
				load = len = 0;

			if( load == 0 )
				break;

			mhash( td, buff, load );
			setProgress( (int)( (double)m_file.pos() * percent + 0.5 ) );
		}


		mhash_deinit( td, hash );
		if( len )
			emit sigAdd( m_name, QByteArray( (const char *)hash, len ).toHex() );

		break;
	}

	delete buff;
	delete hash;
	
	m_file.close();
	setProgress( PROGRESS_SIZE );
}


void QCheckSum::stop( void )
{
	QMutexLocker locker( &m_lock );
	m_stop = true;
}


int QCheckSum::progress( void )
{
	QMutexLocker locker( &m_lock );
	return m_progress;
}


void QCheckSum::setProgress( int prog )
{
	QMutexLocker locker( &m_lock );
	if( m_progress != prog )
	{
		m_progress = prog;
		emit sigProgress();
	}
}
