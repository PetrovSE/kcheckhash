#include <QtGui>
#include "checksum.h"


#define BUFF_SIZE			8192


QCheckSum::QCheckSum( QMainDialog *parent, hashid id, const QString &name, const QString &file ) :
	m_id( id ),
	m_name( name ),
	m_file( file )
{
	m_stop		= false;
	m_progress	= 0;

	connect( this, SIGNAL( emitAdd( const QString &, const QString & ) ), parent, SLOT( onAdd( const QString &, const QString & ) ) );
	connect( this, SIGNAL( emitUpdate( void ) ), parent, SLOT( onUpdate( void ) ) );
	connect( this, SIGNAL( finished( void ) ), parent, SLOT( onFinished( void ) ) );

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

		double percent = (double)PROG_SIZE / ( (double)m_file.size() + 0.1 );


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
			emit emitAdd( m_name, QByteArray( (const char *)hash, len ).toHex() );

		break;
	}

	delete buff;
	delete hash;
	
	m_file.close();
	setProgress( PROG_SIZE );
}


void QCheckSum::stop( void )
{
	m_lock.lock();
	m_stop = true;
	m_lock.unlock();
}


int QCheckSum::progress( void )
{
	m_lock.lock();
	int prog = m_progress;
	m_lock.unlock();
	return prog;
}


void QCheckSum::setProgress( int prog )
{
	bool upd = false;

	m_lock.lock();
	if( m_progress != prog )
	{
		m_progress = prog;
		upd = true;
	}
	m_lock.unlock();

	if( upd )
		emit emitUpdate();
}
