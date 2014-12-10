/* 2013-11-30                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::convertCHUAN_station( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_NumOfFiles )
{
    int         i               = 1;
    int         n               = 0;

    int			stopProgress	= 0;

    int         i_Type          = 0;
    int         i_NumOfBlocks   = 0;

    QString     s_EOL           = setEOLChar( i_EOL );

    QString     s_Test          = "";
    QString     s_QualityFlag   = "";
    QString     s_Data          = "";
    QString     s_Metadata      = "";
    QString     s_Geocode       = "";
    QString     s_GeocodeFlag   = "";

    QString     s_dateflag      = "";
    QString     s_timeflag      = "";

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting CHUAN data..." ), sl_Input.count() );

// **********************************************************************************************

    QFileInfo fi_InFile( s_FilenameIn );

    i_Type = NumOfSections( sl_Input.at( 0 ) );

    switch ( i_Type )
    {
    case 1008:
        i_NumOfBlocks = 50;

        tout << "Event label\t" << "Date/Time\t";
        tout << "Pressure, atmospheric [hPa]\tGeopotential height [gpm]\tTemperature, air [deg C]\tWind direction [deg]\tWind speed [m/s]\t";
        tout << "Wind velocity, west-east [m/s]\tWind velocity, south-north [m/s]\tHumidity, relative [%]\tDew point, difference [K]\t";
        tout << "Humidity, specific [g/kg]\t";
        tout << "QF date []\tQF time []\t";
        tout << "QF atmos press []\tQF geopot h []\tQF air temp []\t";
        tout << "QF wind dir []\tQF wind speed []\tQF U []\t";
        tout << "QF V []\tQF RH []\tQF dew point diff []\t";
        tout << "QF humidity spec []" << s_EOL;
        break;

    case 2008:
        i_NumOfBlocks = 100;

        tout << "Event label\t" << "Date/Time\t";
        tout << "Altitude [m]\tPressure, at given altitude [hPa]\tTemperature, air [deg C]\tWind direction [deg]\tWind speed [m/s]\t";
        tout << "Wind velocity, west-east [m/s]\tWind velocity, south-north [m/s]\tHumidity, relative [%]\tDew point, difference [K]\t";
        tout << "Humidity, specific [g/kg]\t";
        tout << "QF date []\tQF time []\t";
        tout << "QF altitude []\tQF press alt []\tQF air temp []\t";
        tout << "QF wind dir []\tQF wind speed []\tQF U []\t";
        tout << "QF V []\tQF RH []\tQF dew point diff []\t";
        tout << "QF humidity spec []" << s_EOL;
        break;

    default:
        i_NumOfBlocks = 101;

        tout << "Event label\t" << "Date/Time\t";
        tout << "Altitude [m]\tPressure, at given altitude [hPa]\tTemperature, air [deg C]\tWind direction [deg]\tWind speed [m/s]\t";
        tout << "Wind velocity, west-east [m/s]\tWind velocity, south-north [m/s]\tHumidity, relative [%]\tDew point, difference [K]\t";
        tout << "Humidity, specific [g/kg]\t";
        tout << "QF date []\tQF time []\t";
        tout << "QF altitude []\tQF press alt []\tQF air temp []\t";
        tout << "QF wind dir []\tQF wind speed []\tQF U []\t";
        tout << "QF V []\tQF RH []\tQF dew point diff []\t";
        tout << "QF humidity spec []" << s_EOL;
        break;
    }

    while ( ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
    {
        QDateTime s_DateTime( QDate( sl_Input.at( i ).section( "\t", 1, 1 ).toInt(), sl_Input.at( i ).section( "\t", 2, 2 ).toInt(), sl_Input.at( i ).section( "\t", 3, 3 ).toInt() ), QTime( sl_Input.at( i ).section( "\t", 5, 5 ).toInt(), sl_Input.at( i ).section( "\t", 6, 6 ).toInt() ) );

        s_dateflag  = sl_Input.at( i ).section( "\t", 4, 4 );
        s_timeflag  = sl_Input.at( i ).section( "\t", 7, 7 );

        // Metadata
        s_Metadata = "ERA-CLIM_" + fi_InFile.completeBaseName();                     // Event label - 0
        s_Metadata.append( "\t" + s_DateTime.toString( "yyyy-MM-ddThh:mm" ) );       // Date/Time   - 1
        s_Metadata.append( "\t" + s_dateflag );                                      // dateflag    - 2
        s_Metadata.append( "\t" + s_timeflag );                                      // timeflag    - 3
        s_Metadata = s_Metadata.replace( "-999", "" );

        for ( int k=0; k<i_NumOfBlocks; k++ )
        {
            s_Test = sl_Input.at( i ).section( "\t", 7+k*20+3, 7+k*20+20 );

            if ( s_Test.count( "-999" ) < 18 )
            {
                s_Data        = "";
                s_QualityFlag = "";

                s_Geocode     = "\t" + sl_Input.at( i ).section( "\t", 7+k*20+1, 7+k*20+1 );
                s_GeocodeFlag = "\t" + sl_Input.at( i ).section( "\t", 7+k*20+2, 7+k*20+2 );

                for ( int j=3; j<=20; j=j+2 )
                    s_Data.append( "\t" + sl_Input.at( i ).section( "\t", 7+k*20+j, 7+k*20+j ) );

                for ( int j=4; j<=19; j=j+2 )
                   s_QualityFlag.append( "\t" + sl_Input.at( i ).section( "\t", 7+k*20+j, 7+k*20+j ) );

                tout << s_Metadata.section( "\t", 0, 1 ) << s_Geocode.replace( "-999", "" ) << s_Data.replace( "-999", "" ) << "\t";
                tout << s_Metadata.section( "\t", 2, 3 ) << s_GeocodeFlag.replace( "-999", "" ) << s_QualityFlag.replace( "-999", "" ) << s_EOL;
            }
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    fout.close();

    resetProgress( i_NumOfFiles );

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

int MainWindow::convertCHUAN_moving( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_NumOfFiles )
{
    int         i               = 1;
    int         n               = 0;

    int			stopProgress	= 0;

    int         i_Type          = 0;
    int         i_NumOfBlocks   = 0;

    QString     s_EOL           = setEOLChar( i_EOL );

    QString     s_Test          = "";
    QString     s_QualityFlag   = "";
    QString     s_Data          = "";
    QString     s_Metadata      = "";
    QString     s_Geocode       = "";
    QString     s_GeocodeFlag   = "";

    QString     s_Latitude      = "";
    QString     s_Longitude     = "";

    QString     s_coordflag     = "";
    QString     s_dateflag      = "";
    QString     s_timeflag      = "";

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting CHUAN data..." ), sl_Input.count() );

// **********************************************************************************************

    QFileInfo fi_InFile( s_FilenameIn );

    i_Type = NumOfSections( sl_Input.at( 0 ) );

    switch ( i_Type )
    {
    case 1011:
        i_NumOfBlocks = 50;

        tout << "Event label\tDate/Time\tLatitude\tLongitude\t";
        tout << "Pressure, atmospheric [hPa]\tGeopotential height [gpm]\tTemperature, air [deg C]\tWind direction [deg]\tWind speed [m/s]\t";
        tout << "Wind velocity, west-east [m/s]\tWind velocity, south-north [m/s]\tHumidity, relative [%]\tDew point, difference [K]\t";
        tout << "Humidity, specific [g/kg]\t";
        tout << "QF pos []\tQF date []\tQF time []\t";
        tout << "QF atmos press []\tQF geopot h []\tQF air temp []\t";
        tout << "QF wind dir []\tQF wind speed []\tQF U []\t";
        tout << "QF V []\tQF RH []\tQF dew point diff []\t";
        tout << "QF humidity spec []" << s_EOL;
        break;

    case 2011:
        i_NumOfBlocks = 100;

        tout << "Event label\tDate/Time\tLatitude\tLongitude\t";
        tout << "Altitude [m]\tPressure, at given altitude [hPa]\tTemperature, air [deg C]\tWind direction [deg]\tWind speed [m/s]\t";
        tout << "Wind velocity, west-east [m/s]\tWind velocity, south-north [m/s]\tHumidity, relative [%]\tDew point, difference [K]\t";
        tout << "Humidity, specific [g/kg]\t";
        tout << "QF pos []\tQF date []\tQF time []\t";
        tout << "QF altitude []\tQF press alt []\tQF air temp []\t";
        tout << "QF wind dir []\tQF wind speed []\tQF U []\t";
        tout << "QF V []\tQF RH []\tQF dew point diff []\t";
        tout << "QF humidity spec []" << s_EOL;
        break;

    default:
        i_NumOfBlocks = 12;

        tout << "Event label\tDate/Time\tLatitude\tLongitude\t";
        tout << "Altitude [m]\tPressure, at given altitude [hPa]\tTemperature, air [deg C]\tWind direction [deg]\tWind speed [m/s]\t";
        tout << "Wind velocity, east-west [m/s]\tWind velocity, south-north [m/s]\tHumidity, relative [%]\tDew point difference [K]\tHumidity, specific [g/kg]\t";
        tout << "QF pos []\tQF date []\tQF time []\t";
        tout << "QF altitude []\tQF press alt []\tQF air temp []\t";
        tout << "QF wind dir []\tQF wind speed []\tQF U []\t";
        tout << "QF V []\tQF RH []\tQF dew point diff []\t";
        tout << "QF humidity spec []" << s_EOL;
        break;
    }

    while ( ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
    {
        QDateTime s_DateTime( QDate( sl_Input.at( i ).section( "\t", 4, 4 ).toInt(), sl_Input.at( i ).section( "\t", 5, 5 ).toInt(), sl_Input.at( i ).section( "\t", 6, 6 ).toInt() ), QTime( sl_Input.at( i ).section( "\t", 8, 8 ).toInt(), sl_Input.at( i ).section( "\t", 9, 9 ).toInt() ) );

        s_Latitude  = sl_Input.at( i ).section( "\t", 1, 1 );
        s_Longitude = sl_Input.at( i ).section( "\t", 2, 2 );
        s_coordflag = sl_Input.at( i ).section( "\t", 3, 3 );
        s_dateflag  = sl_Input.at( i ).section( "\t", 7, 7 );
        s_timeflag  = sl_Input.at( i ).section( "\t", 10, 10 );

        // Metadata
        s_Metadata = "ERA-CLIM_" + fi_InFile.completeBaseName();                     // Event label - 0
        s_Metadata.append( "\t" + s_DateTime.toString( "yyyy-MM-ddThh:mm" ) );       // Date/Time   - 1
        s_Metadata.append( "\t" + s_Latitude );                                      // Latitude    - 2
        s_Metadata.append( "\t" + s_Longitude );                                     // Longitude   - 3
        s_Metadata.append( "\t" + s_coordflag );                                     // coordflag   - 4
        s_Metadata.append( "\t" + s_dateflag );                                      // dateflag    - 5
        s_Metadata.append( "\t" + s_timeflag );                                      // timeflag    - 6
        s_Metadata = s_Metadata.replace( "-999", "" );

        for ( int k=0; k<i_NumOfBlocks; k++ )
        {
            s_Test = sl_Input.at( i ).section( "\t", 10+k*20+3, 10+k*20+20 );

            if ( s_Test.count( "-999" ) < 18 )
            {
                s_Data        = "";
                s_QualityFlag = "";

                s_Geocode     = "\t" + sl_Input.at( i ).section( "\t", 10+k*20+1, 10+k*20+1 );
                s_GeocodeFlag = "\t" + sl_Input.at( i ).section( "\t", 10+k*20+2, 10+k*20+2 );

                for ( int j=3; j<=19; j=j+2 )
                    s_Data.append( "\t" + sl_Input.at( i ).section( "\t", 10+k*20+j, 10+k*20+j ) );

                for ( int j=4; j<=20; j=j+2 )
                   s_QualityFlag.append( "\t" + sl_Input.at( i ).section( "\t", 10+k*20+j, 10+k*20+j ) );

                tout << s_Metadata.section( "\t", 0, 3 ) << s_Geocode.replace( "-999", "" ) << s_Data.replace( "-999", "" ) << "\t";
                tout << s_Metadata.section( "\t", 4, 6 ) << s_GeocodeFlag.replace( "-999", "" ) << s_QualityFlag.replace( "-999", "" ) << s_EOL;
            }
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

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

void MainWindow::doConvertCHUAN_station()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Convert CHUAN data..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = convertCHUAN_station( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gsl_FilenameList.count() );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting CHUAN data was canceled" ) );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-05-31

void MainWindow::doConvertCHUAN_moving()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Convert CHUAN data..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = convertCHUAN_moving( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gsl_FilenameList.count() );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting CHUAN data was canceled" ) );

    onError( err );
}

