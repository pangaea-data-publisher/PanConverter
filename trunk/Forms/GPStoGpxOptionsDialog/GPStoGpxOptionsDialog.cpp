// ***********************************************************************************************
// *                                                                                             *
// * GPStoGpxOptionsDialog.cpp - Set the options for the GPS to GPX converter                    *
// *                                                                                             *
// * Dr. Rainer Sieger - 2009-03-23                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "GPStoGpxOptionsDialog.h"

GPStoGpxOptionsDialog::GPStoGpxOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doGPStoGpxOptionsDialog( int &i_StartID, int &i_EndID, QString &s_TrackName )
{
    int i_DialogResult = QDialog::Rejected;

    GPStoGpxOptionsDialog dialog( this );

    dialog.StartID_spinBox->setValue( i_StartID );
    dialog.EndID_spinBox->setValue( i_EndID );
    dialog.TrackName_lineEdit->setText( s_TrackName );

    dialog.StartID_spinBox->selectAll();

    dialog.TrackName_lineEdit->selectAll();

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        i_StartID       = dialog.StartID_spinBox->value();
        i_EndID         = dialog.EndID_spinBox->value();
        s_TrackName     = dialog.TrackName_lineEdit->text();

        i_DialogResult  = QDialog::Accepted;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

    return( i_DialogResult );
}
