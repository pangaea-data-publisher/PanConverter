// ***********************************************************************************************
// *                                                                                             *
// * GPStoKmlOptionsDialog.cpp - Set the options for the GPS to KML converter                    *
// *                                                                                             *
// * Dr. Rainer Sieger - 2010-08-16                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include <QtWidgets>

#include "Application.h"
#include "GPStoKmlOptionsDialog.h"

GPStoKmlOptionsDialog::GPStoKmlOptionsDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );

    connect(OK_pushButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(Cancel_pushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(BrowseGoogleEarthProgram_pushButton, SIGNAL(clicked()), this, SLOT(browseFilenameDialog()));
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

int MainWindow::doGPStoKmlOptionsDialog( int &i_StartID, int &i_EndID,
                                         int &i_IconColor, int &i_IconSize,
                                         int &i_TracklineColor, int &i_TracklineWidth,
                                         QString &s_TrackName,
                                         QString &s_FilenameGoogleEarthProgram, bool &b_startGoogleEarth )
{
    int i_DialogResult = QDialog::Rejected;

// ***********************************************************************************************************************

    GPStoKmlOptionsDialog dialog( this );

    dialog.StartID_spinBox->setValue( i_StartID );
    dialog.EndID_spinBox->setValue( i_EndID );
    dialog.TrackName_lineEdit->setText( s_TrackName );
    dialog.IconColor_comboBox->setCurrentIndex( i_IconColor );
    dialog.IconSize_spinBox->setValue( i_IconSize );
    dialog.TracklineColor_comboBox->setCurrentIndex( i_TracklineColor );
    dialog.TracklineWidth_spinBox->setValue( i_TracklineWidth );
    dialog.GoogleEarthProgram_lineEdit->setText( QDir::toNativeSeparators( s_FilenameGoogleEarthProgram ) );
    dialog.StartGoogleEarth_checkBox->setChecked( b_startGoogleEarth );

    dialog.StartID_spinBox->selectAll();

    dialog.OK_pushButton->setWhatsThis( "Close dialog" );
    dialog.Cancel_pushButton->setWhatsThis( "Cancel dialog" );
    dialog.BrowseGoogleEarthProgram_pushButton->setWhatsThis( "Browse for the Google Earth program file" );

#if defined(Q_OS_MAC)
    dialog.StartGoogleEarth_checkBox->setChecked( false );
    dialog.GoogleEarth_groupBox->hide();
#endif

    dialog.TrackName_lineEdit->selectAll();

    dialog.move( posDialog );
    dialog.resize( dialog.sizeHint() );
    dialog.show();

// ******************************************************************************************************

    switch ( dialog.exec() )
    {
    case QDialog::Accepted:
        i_StartID                     = dialog.StartID_spinBox->value();
        i_EndID                       = dialog.EndID_spinBox->value();
        i_IconColor                   = dialog.IconColor_comboBox->currentIndex();
        i_IconSize                    = dialog.IconSize_spinBox->value();
        i_TracklineColor              = dialog.TracklineColor_comboBox->currentIndex();
        i_TracklineWidth              = dialog.TracklineWidth_spinBox->value();
        s_TrackName                   = dialog.TrackName_lineEdit->text();
        b_startGoogleEarth            = dialog.StartGoogleEarth_checkBox->isChecked();
        s_FilenameGoogleEarthProgram  = dialog.GoogleEarthProgram_lineEdit->text();

        i_DialogResult                = QDialog::Accepted;
        break;

    case QDialog::Rejected:
        break;

    default:
        break;
    }

    posDialog = dialog.pos();

    return( i_DialogResult );
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

void GPStoKmlOptionsDialog::browseFilenameDialog()
{
    QString	fn			= "";
    QString file		= this->GoogleEarthProgram_lineEdit->text();
    QString fileStart	= getProgramDir();

// ***********************************************************************************************************************

    fileStart.replace( "://", ":/" );

    if ( ( file.startsWith( "Browse" ) == false ) && ( file.isEmpty() == false ) )
        fileStart = file;

    #if defined(Q_OS_WIN)
        fn = QFileDialog::getOpenFileName( this, tr( "Select the Google Earth program" ), fileStart, tr( "EXE file (*.exe)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_LINUX)
        fn = QFileDialog::getOpenFileName( this, tr( "Select the Google Earth program" ), fileStart, tr( "All (*)" ), 0, QFileDialog::DontUseNativeDialog );
    #endif

    #if defined(Q_OS_MAC)
        fn = QFileDialog::getExistingDirectory( this, tr( "Select the Google Earth program" ), fileStart, QFileDialog::DontUseNativeDialog );
    #endif

    if ( fn.isEmpty() == true )
        fn = file;

    this->GoogleEarthProgram_lineEdit->setText( QDir::toNativeSeparators( fn ) );

    GoogleEarthProgram_lineEdit->setFocus();
}

// ***********************************************************************************************************************
// ***********************************************************************************************************************
// ***********************************************************************************************************************

QString GPStoKmlOptionsDialog::getProgramDir()
{
    #if defined(Q_OS_LINUX)
        return( QDir::homePath() );
    #endif

    #if defined(Q_OS_MAC)
        return( "/Applications" );
    #endif

    #if defined(Q_OS_WIN)
        return( QDir::rootPath() + QLatin1String( "Program Files (x86)") );
    #endif
}
