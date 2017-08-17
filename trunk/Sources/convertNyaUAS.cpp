/* 2017-08-15                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::convertNyaUAS( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_outputType, const int i_NumOfFiles )
{
    int         i               = 1;
    int         n               = 0;

    int			stopProgress	= 0;

    int         i_Type          = 1; // with Ozone

    bool        b_datafound     = false;

    QString     s_EventLabel    = "";
    QString     tempStr         = "";
    QString     s_EOL           = setEOLChar( i_EOL );

    QStringList sl_Input;

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( -10 );

// **********************************************************************************************
// open output file

    QFile fout( s_FilenameOut );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &fout );

    switch ( i_CodecOutput )
    {
    case _SYSTEM_:
        break;
    case _LATIN1_:
        tout.setCodec( QTextCodec::codecForName( "ISO 8859-1" ) );
        break;
    case _APPLEROMAN_:
        tout.setCodec( QTextCodec::codecForName( "Apple Roman" ) );
        break;
    default:
        tout.setCodec( QTextCodec::codecForName( "UTF-8" ) );
        break;
    }

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting NYA upper air soundings..." ), sl_Input.count() );

// **********************************************************************************************

    while ( ( b_datafound == false ) && ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( sl_Input.at( i ).contains( "End of Header Information") == true )
            b_datafound = true;

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    if ( b_datafound == true )
    {
        QDateTime DateTimeStart( QDate( sl_Input.at( 6 ).section( " ", 0, 0 ).toInt(), sl_Input.at( 6 ).section( " ", 1, 1 ).toInt(), sl_Input.at( 6 ).section( " ", 2, 2 ).toInt() ));

        DateTimeStart = DateTimeStart.addSecs( (qint64) ( sl_Input.at( ++i ).section( " ", 6, 6 ).toFloat() * 3600. ) );

        s_EventLabel = QString( "NYA@%1-%2" ).arg( sl_Input.at( 6 ).section( " ", 0, 0 ).toInt(), 4, 10, QLatin1Char( '0' ) ).arg( sl_Input.at( 6 ).section( " ", 1, 1 ).toInt(), 2, 10, QLatin1Char( '0' ) );

        while ( ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( sl_Input.at( i ).startsWith( "ElapTime" ) == true )
            {
                if ( sl_Input.at( 3 ).contains( "ECC" ) == false )
                    i_Type = 0;

                if ( ( sl_Input.at( 3 ).contains( "ECC" ) == true ) && ( i_outputType == 0 ) )
                    i_Type = 0;

                if ( i_Type == 0)
                {
                    tout << "Event label\tDate/Time\tLatitude\tLongitude\tAltitude [m]\tHeight, geometric [m]\tElapsed time [s]\t";
                    tout << "Pressure at given altitude [m]\tTemperature, air [deg C]\tHumidity, relative [%]\t";
                    tout << "Wind direction [deg]\tWind speed [m/s]" << s_EOL;
/*
                    Pressure at observation [hPa], combined uncertainty 1.0 hPa (>100hPa), 0.6 hPa (100-3 hPa) - Default: 9999.99
                    Geopotential height above sea level [m], integrated from pressure and temperature - Default: 99999
                    Temperature [K], measured using a radiosonde,  combined uncertainty 0.2-0.4 K - Default: 999.9
                    Relative Humidity [%], combined uncertainty 3-4% - Default: 999
                    Ozone partial pressure [mPa], derived - Default: 99.99
                    Horizontal wind direction [degrees] - Default: 999
                    Horizontal wind speed [m/s] - Default: 999.9
                    GPS geometric height [m] - Default: 99999
                    GPS longitude [decimal degrees E] (range: 0.00 - 359.99) - Default: 999.999
                    GPS latitude [decimal degrees N] - Default: 99.999
*/
                }
                else
                {
                    tout << "Event label\tDate/Time\tLatitude\tLongitude\tAltitude [m]\tHeight, geometric [m]\tElapsed time [s]\t";
                    tout << "Pressure at given altitude [m]\tTemperature, air [deg C]\tHumidity, relative [%]\tOzone, partial pressure [mPa]\t";
                    tout << "Wind direction [deg]\tWind speed [m/s]" << s_EOL;
/*
                    Pressure at observation [hPa], combined uncertainty 1.0 hPa (>100hPa), 0.6 hPa (100-3 hPa) - Default: 9999.99
                    Geopotential height above sea level [m], integrated from pressure and temperature - Default: 99999
                    Temperature [K], measured using a radiosonde,  combined uncertainty 0.2-0.4 K - Default: 999.9
                    Relative Humidity [%], combined uncertainty 3-4% - Default: 999
                    Ozone partial pressure [mPa], derived - Default: 99.99
                    Horizontal wind direction [degrees] - Default: 999
                    Horizontal wind speed [m/s] - Default: 999.9
                    GPS geometric height [m] - Default: 99999
                    GPS longitude [decimal degrees E] (range: 0.00 - 359.99) - Default: 999.999
                    GPS latitude [decimal degrees N] - Default: 99.999
                    Internal temperature [K] (pump) - Default: 999.99
                    Ozone raw current [microA] - Default: 999.9
                    Battery voltage [V] - Default: 999.9
                    Pump current [mA] - Default: 999.9
*/
                }

                break;
            }

            stopProgress = incProgress( i_NumOfFiles, ++i );
        }

        i +=2 ;

        while ( ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
        {
            tempStr = sl_Input.at( i ).simplified();
            tempStr.replace( " ", "\t" );

            tout << s_EventLabel << "\t" << DateTimeStart.addSecs( tempStr.section( "\t", 0, 0 ).toLong() ).toString( Qt::ISODate ) << "\t";

            if ( tempStr.section( "\t", 10, 10 ) != "99.999" ) // Latitude - GPS
                tout << tempStr.section( "\t",10, 10 );

            tout << "\t";

            if ( tempStr.section( "\t", 9, 9 ) != "999.999" ) // Longitude - GPS
            {
                if ( tempStr.section( "\t", 9, 9 ).toFloat() < 180. )
                    tout << tempStr.section( "\t", 9, 9 );
                else
                    tout << QString( "%1" ).arg( tempStr.section( "\t", 9, 9 ).toFloat()-360., 0, 'f', 3 );
            }

            tout << "\t";

            if ( tempStr.section( "\t", 2, 2 ) != "99999" ) // Altitude [m] - Geopotential height above sea level
                tout << tempStr.section( "\t", 2, 2 );

            tout << "\t";

            if ( tempStr.section( "\t", 8, 8 ) != "99999" ) // Height, geometric [m] - GPS
                tout << tempStr.section( "\t", 8, 8 );

            tout << "\t";

            tout << tempStr.section( "\t", 0, 0 ); // Elapsed time

            tout << "\t";

            if ( tempStr.section( "\t", 1, 1 ) != "9999.99" ) // Pressure at given altitude
                tout << tempStr.section( "\t", 1, 1 );

            tout << "\t";

            if ( tempStr.section( "\t", 3, 3 ) != "999.9" ) // Temperature, air
                tout << QString( "%1" ).arg( tempStr.section( "\t", 3, 3 ).toFloat()-273.15, 0, 'f', 2 );

            tout << "\t";

            if ( tempStr.section( "\t", 4, 4 ) != "999" ) // Relative Humidity
                tout << tempStr.section( "\t", 4, 4 );

            tout << "\t";

            if ( ( i_Type == 1 ) && ( tempStr.section( "\t", 5, 5 ) != "99.99" ) ) // Ozone, partial pressure
                tout << tempStr.section( "\t", 5, 5 ) << "\t";

            if ( tempStr.section( "\t", 6, 6 ) != "999" ) // Horizontal wind direction
                tout << tempStr.section( "\t", 6, 6 );

            tout << "\t";

            if ( tempStr.section( "\t", 7, 7 ) != "999.9" ) // Horizontal wind speed
                tout << tempStr.section( "\t", 7, 7 ) << s_EOL;

            s_EventLabel.clear();

            stopProgress = incProgress( i_NumOfFiles, ++i );
        }
    }
    else
    {
        tout << "No data found! Wrong format." << s_EOL;
    }

// **********************************************************************************************

    fout.close();

    resetProgress( i_NumOfFiles );

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2017-08-17

void MainWindow::doConvertNyaUAS_withoutOzone()
{
    convertNyaUAS( 0 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2017-08-17

void MainWindow::doConvertNyaUAS_withOzone()
{
    convertNyaUAS( 1 );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2017-08-17

void MainWindow::convertNyaUAS( const int i_outputType )
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Converting NYA upper air soundings..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                QFileInfo fi( s_FilenameIn );

                s_FilenameOut = fi.absolutePath() + "/" + fi.baseName() + "_" + fi.suffix() + setExtension( gi_Extension ) ;

                err = convertNyaUAS( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, i_outputType, gsl_FilenameList.count() );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting NYA upper air soundings was canceled" ), true, false );

    onError( err );
}

