#ifndef _QT4_TO_QT5_H_
#define _QT4_TO_QT5_H_

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK( 5, 0, 0 )
#define INCL_GUI	<QtWidgets>
#else
#define INCL_GUI	<QtGui>
#endif

#endif // _QT4_TO_QT5_H_
