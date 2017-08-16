/* 2017-08-15                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::convertNyaUAS( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_NumOfFiles )
{
    int         i               = 1;
    int         n               = 0;

    int			stopProgress	= 0;

    int         i_Type          = 0;

    bool        b_datafound     = false;

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

        while ( ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( sl_Input.at( i ).startsWith( "ElapTime" ) == true )
            {
                if ( sl_Input.at( i ).contains( "Ipmp" ) == false )
                {
                    i_Type = 0;

                    tout << "Event label\tDate/Time\tLatitude\tLongitude\tAltitude [m]\tElapsed time [s]\tPressure at given altitude [m]\t";
                    tout << "Geopotential height above sea level [m]\tTemperature, air [deg C]\tHumidity, relative [%]\t";
                    tout << "Wind direction [deg]\tWind speed [m/s]" << s_EOL;
                }
                else
                {
                    i_Type = 1;

                    tout << "Event label\tDate/Time\tLatitude\tLongitude\tAltitude [m]\tElapsed time [s]\tPressure at given altitude [m]\t";
                    tout << "Geopotential height above sea level [m]\tTemperature, air [deg C]\tHumidity, relative [%]\tOzone, partial pressure [mPa]\t";
                    tout << "Wind direction [deg]\tWind speed [m/s]" << s_EOL;
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
            tempStr.replace( "\t99.99\t", "\t\t" );

            tout << "NYA" << "\t" << DateTimeStart.addSecs( tempStr.section( "\t", 0, 0 ).toLong() ).toString( Qt::ISODate ) << "\t";

            tout << tempStr.section( "\t", 9, 10 ) << "\t" << tempStr.section( "\t", 8, 8 ) << "\t";
            tout << tempStr.section( "\t", 0, 0 ) << "\t" << tempStr.section( "\t", 1, 2 ) << "\t";
            tout << QString( "%1" ).arg( tempStr.section( "\t", 3, 3 ).toFloat()-273.15, 0, 'f', 2 ) << "\t";

            if ( i_Type == 0 )
                tout << tempStr.section( "\t", 4, 4 ) << "\t" << tempStr.section( "\t", 6, 7 ) << s_EOL;
            else
                tout << tempStr.section( "\t", 4, 7 ) << s_EOL;

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
// 2011-05-31

void MainWindow::doConvertNyaUAS()
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
                err = convertNyaUAS( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gsl_FilenameList.count() );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting NYA upper air soundings was canceled" ) );

    onError( err );
}

