/* 2008-04-07                 */
/* Dr. Rainer Sieger          */

#include <QtGui>

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::WOCEWHPConverter( const QString &s_FilenameIn, const QString &s_FilenameOut )
{
    int         i               = 0;
    int         n               = 0;

    QString     s_Event         = "";

    QStringList sl_Input;

// **********************************************************************************************
// open output file

    QFile fout( s_FilenameOut );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &fout );

// **********************************************************************************************
// read file

    n = readFile( s_FilenameIn, sl_Input );

// **********************************************************************************************

    QFileInfo fi( s_FilenameIn );
    s_Event = fi.baseName();
    s_Event.replace( "~", "/" );

    while ( ( sl_Input.at(i).startsWith( "DATE" ) == false ) && ( i < 30 ) )
        i++;

    QString s_Date      = sl_Input.at(i++).section( " = ", 1, 1 ).simplified();
    QString s_Time      = sl_Input.at(i++).section( " = ", 1, 1 ).simplified();
    QString s_Latitude  = sl_Input.at(i++).section( " = ", 1, 1 ).simplified();
    QString s_Longitude = sl_Input.at(i++).section( " = ", 1, 1 ).simplified();
    QString s_Elevation = sl_Input.at(i++).section( " = ", 1, 1 ).simplified();

    QString s_DateTime = s_Date.left( 4 ) + "-" + s_Date.mid( 4, 2 ) + "-" + s_Date.right( 2 ) + "T" + s_Time.left( 2 ) + ":" + s_Time.right( 2 );

    while ( ( sl_Input.at(i).startsWith( "   " ) == false ) && ( i < 30 ) )
        i++;

    tout << "Event label\tDate/Time\tLatitude\tLongitude\tBot. Depth [m]\tPressure, water [dbar]\tTemperature, water [deg C]\tSalinity []\tOxygen [mymol/kg]" << endl;

    while ( ( sl_Input.at(i).startsWith( "   " ) == true ) && ( i < sl_Input.count() ) )
    {
        QString s_Pressure        = sl_Input.at(i).section( ",", 0, 0 ).simplified();
        QString s_PressureFlag    = sl_Input.at(i).section( ",", 1, 1 ).simplified();

        QString s_Temperature     = sl_Input.at(i).section( ",", 2, 2 ).simplified();
        QString s_TemperatureFlag = sl_Input.at(i).section( ",", 3, 3 ).simplified();

        QString s_Salinity        = sl_Input.at(i).section( ",", 4, 4 ).simplified();
        QString s_SalinityFlag    = sl_Input.at(i).section( ",", 5, 5 ).simplified();

        QString s_Oxygen          = sl_Input.at(i).section( ",", 6, 6 ).simplified();
        QString s_OxygenFlag      = sl_Input.at(i).section( ",", 7, 7 ).simplified();

        if ( s_PressureFlag == "9" ) s_Pressure = "";
        if ( s_TemperatureFlag == "9" ) s_Temperature = "";
        if ( s_SalinityFlag == "9" ) s_Salinity = "";
        if ( s_OxygenFlag == "9" ) s_Oxygen = "";

        convertQualityFlag( s_PressureFlag );
        convertQualityFlag( s_TemperatureFlag );
        convertQualityFlag( s_SalinityFlag );
        convertQualityFlag( s_OxygenFlag );

        tout << s_Event << "\t" << s_DateTime << "\t" << s_Latitude << "\t" << s_Longitude << "\t" << s_Elevation << "\t" << s_Pressure << "\t";
        tout << s_TemperatureFlag << s_Temperature << "\t" << s_SalinityFlag << s_Salinity << "\t" << s_OxygenFlag << s_Oxygen << "\t" << endl;

        i++;
    }

    fout.close();

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-13

int MainWindow::convertQualityFlag( QString &s_QualityFlag )
{
    s_QualityFlag.replace( "1", "?" );  // questionable
    s_QualityFlag.replace( "2", "" );   // good
    s_QualityFlag.replace( "3", "?" );  // questionable
    s_QualityFlag.replace( "4", "/" );  // not valid
    s_QualityFlag.replace( "5", "*" );  // unknown
    s_QualityFlag.replace( "6", "#" );  // interpolated
    s_QualityFlag.replace( "7", "*" );  // unknown
    s_QualityFlag.replace( "8", "*" );  // unknown
    s_QualityFlag.replace( "9", "" );   // not value

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doWOCEWHPConverter()
{
    int		i				= 0;
    int		err				= 0;
    int		stopProgress	= 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "WOCE-WHP converter working..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = WOCEWHPConverter( s_FilenameIn, s_FilenameOut );

                stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
            }
            else
            {
                err = _FILENOTEXISTS_;
            }
        }

        resetFileProgress( gsl_FilenameList.count() );
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "WOCE-WHP converter was canceled" ) );

    onError( err );
}

