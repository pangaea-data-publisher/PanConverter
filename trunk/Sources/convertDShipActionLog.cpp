/* 2017-03-04                 */
/* Dr. Rainer Sieger          */

#include "Application.h"
#include "convertPosition.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2017-08-29

int MainWindow::convertDShipActionLogCruiseReport( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_NumOfFiles )
{
    int         i               = 1;
    int         n               = 0;

    int			stopProgress	= 0;

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting DShip ActionLog..." ), sl_Input.count() );

// **********************************************************************************************

    tout << "Campaign" << "\t" << "Station" << "\t" << "Date" << "\t" << "Time" << "\t";
    tout << "Position Latitude" << "\t" << "Position Longitude" << "\t" << "Depth (m)" << "\t";
    tout << "Gear" << "\t" << "Action" << "\t" << "Comment" << "\t" << "Action flag" << s_EOL;

// **********************************************************************************************

    const int STATIONSTART = 1;
    const int PROFILESTART = 2;
    const int ATDEPTH      = 3;
    const int PROFILEEND   = 4;
    const int STATIONEND   = 5;

    while ( ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
    {
        int     i_Action   = 0;
        QString s_DateTime = "";

        if ( sl_Input.at( i ).section( "\t", 6, 6 ).isEmpty() == false )
        {
            i_Action   = STATIONSTART;
            s_DateTime = sl_Input.at( i ).section( "\t", 6, 6 );
        }

        if ( sl_Input.at( i ).section( "\t", 11, 11 ).isEmpty() == false )
        {
            i_Action   = PROFILESTART;
            s_DateTime = sl_Input.at( i ).section( "\t", 11, 11 );
        }

        if ( sl_Input.at( i ).section( "\t", 16, 16 ).isEmpty() == false )
        {
            i_Action   = ATDEPTH;
            s_DateTime = sl_Input.at( i ).section( "\t", 26, 26 );
        }

        if ( sl_Input.at( i ).section( "\t", 26, 26 ).isEmpty() == false )
        {
            i_Action   = PROFILEEND;
            s_DateTime = sl_Input.at( i ).section( "\t", 26, 26 );
        }

        if ( sl_Input.at( i ).section( "\t", 21, 21 ).isEmpty() == false )
        {
            i_Action   = STATIONEND;
            s_DateTime = sl_Input.at( i ).section( "\t", 21, 21 );
        }

        tout << sl_Input.at( i ).section( "\t", 0, 1 ) << "\t"; // Campaign, Station

        tout << s_DateTime.section( " ", 0, 0 ).replace( "/", "-" ) << "\t"; // Date
        tout << s_DateTime.section( " ", 1, 1 ).section( ":", 0, 2 ) << "\t"; // Time
        tout << sl_Input.at( i ).section( "\t", 7, 9 ) << "\t"; // Latitude, Longitude, Water depth
        tout << sl_Input.at( i ).section( "\t", 5, 5 ) << "\t"; // Device Code

        switch ( i_Action )
        {
        case STATIONSTART:
            tout << "Station Start" << "\t" << STATIONSTART;
            break;

        case PROFILESTART:
            tout << "Profile Start" << "\t" << STATIONEND;
            break;

        case ATDEPTH:
            tout << "at Depth" << "\t" << ATDEPTH;
            break;

        case PROFILEEND:
            tout << "Profile End" << "\t" << PROFILEEND;
            break;

        case STATIONEND:
            tout << "Station End" << "\t" << STATIONEND;
            break;

        default:
            tout << "" << "\t" << "0";
            break;
        }

        tout << s_EOL;

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
// 2017-04-19

int MainWindow::convertDShipActionLog( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_NumOfFiles )
{
    int         i               = 1;
    int         n               = 0;

    int         i_first         = 0;
    int         i_last          = 0;

    int			stopProgress	= 0;

    int         i_Format        = -999;

    int			i_Campaign      = 0;
    int			i_EventLabel    = 0;
    int			i_DateTime      = 1;
    int			i_DeviceLong    = 2;
    int			i_DeviceAbbr    = 3;
    int			i_Action        = 4;
    int			i_Latitude      = 9;
    int			i_Longitude     = 10;
    int			i_Elevation     = 11;

    QString     s_CampaignLabel = "";
    QString     s_EventLabel    = "";
    QString     s_Device        = "";

    QString     s_DateTime1     = "";
    QString     s_Latitude1     = "";
    QString     s_Longitude1    = "";
    QString     s_Elevation1    = "";

    QString     s_DateTime2     = "";
    QString     s_Latitude2     = "";
    QString     s_Longitude2    = "";
    QString     s_Elevation2    = "";

    QString     s_EOL           = setEOLChar( i_EOL );

    QStringList sl_Input;

    convertPosition *llc = new convertPosition( 5, _DOT_ );

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( -10 );

// **********************************************************************************************
// check format

    if ( ( sl_Input.at( 0 ).section( "\t", 9, 9 ).startsWith( "Latitude" ) == true ) && ( sl_Input.at( 0 ).section( "\t", 10, 10 ).startsWith( "Longitude" ) == true ) ) // BSH
    {
        i_Format     = _BSH_;
        i_Latitude   = 9;
        i_Longitude  = 10;
    }

    if ( ( sl_Input.at( 0 ).section( "\t", 14, 14 ).startsWith( "Latitude" ) == true ) && ( sl_Input.at( 0 ).section( "\t", 15, 15 ).startsWith( "Longitude" ) == true ) ) // AWI
    {
        i_Format     = _AWI_;
        i_Latitude   = 14;
        i_Longitude  = 15;
    }

    if ( i_Format < 0 )
        return( -40 );

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting DShip ActionLog..." ), sl_Input.count() );

// **********************************************************************************************

    tout << "Campaign" << "\t" << "LabelEvent" << "\t" << "Device" << "\t" << "Device (long)" << "\t" << "Action" << "\t" << "DateTimeEvent" << "\t";
    tout << "LatitudeEvent" << "\t" << "LongitudeEvent" << "\t" << "ElevationEvent" << "\t";
    tout << "Action2" << "\t" << "DateTimeEvent2" << "\t" << "LatitudeEvent2" << "\t" << "LongitudeEvent2" << "\t" << "ElevationEvent2" << s_EOL;

// **********************************************************************************************

    i_first = 1;
    i       = 2;

    while ( ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( sl_Input.at( i-1 ).section( "\t", 0, 0 ) != sl_Input.at( i ).section( "\t", 0, 0 ) )
        {
            i_last  = i-1;

            for ( int j = i_first; j <= i_last; j++ )
            {
                if ( sl_Input.at( j ).contains( "station start" ) == true )
                    i_first = j;

                if ( sl_Input.at( j ).contains( "profile start" ) == true )
                    i_first = j;

                if ( sl_Input.at( j ).contains( "station end" ) == true )
                    i_last = j;

                if ( sl_Input.at( j ).contains( "profile end" ) == true )
                    i_last = j;

                if ( sl_Input.at( j ).contains( "max depth/on ground" ) == true )
                {
                    i_first = j;
                    i_last  = j;
                }
            }

            s_CampaignLabel = sl_Input.at( i_first ).section( "\t", i_Campaign, i_Campaign ).section( "_", 0, 0 );
            s_EventLabel    = sl_Input.at( i_first ).section( "\t", i_EventLabel, i_EventLabel );

            s_EventLabel.replace( "_Underway", "_underway" );
            s_EventLabel.replace( "_00_", "_0_" );

            s_DateTime1 = sl_Input.at( i_first ).section( "\t", i_DateTime, i_DateTime ).left( 16 );
            s_DateTime1.replace( "/", "-" );
            s_DateTime1.replace( " ", "T" );

            s_DateTime2 = sl_Input.at( i_last ).section( "\t", i_DateTime, i_DateTime ).left( 16 );
            s_DateTime2.replace( "/", "-" );
            s_DateTime2.replace( " ", "T" );

            switch ( i_Format )
            {
            case _AWI_:
                s_Latitude1  = QString( "%1" ).arg( sl_Input.at( i_first ).section( "\t", i_Latitude, i_Latitude ).toDouble(), 0, 'f', 5 );
                s_Longitude1 = QString( "%1" ).arg( sl_Input.at( i_first ).section( "\t", i_Longitude, i_Longitude ).toDouble(), 0, 'f', 5 );

                s_Latitude2  = QString( "%1" ).arg( sl_Input.at( i_last ).section( "\t", i_Latitude, i_Latitude ).toDouble(), 0, 'f', 5 );
                s_Longitude2 = QString( "%1" ).arg( sl_Input.at( i_last ).section( "\t", i_Longitude, i_Longitude ).toDouble(), 0, 'f', 5 );

                break;

            case _BSH_:
                llc->convertLLtoLL( sl_Input.at( i_first ).section( "\t", i_Latitude, i_Latitude ), sl_Input.at( i_first ).section( "\t", i_Longitude, i_Longitude ) );

                s_Latitude1  = llc->Latitude();
                s_Longitude1 = llc->Longitude();

                llc->convertLLtoLL( sl_Input.at( i_last ).section( "\t", i_Latitude, i_Latitude ), sl_Input.at( i_last ).section( "\t", i_Longitude, i_Longitude ) );

                s_Latitude2  = llc->Latitude();
                s_Longitude2 = llc->Longitude();
            }

            s_Elevation1 = "";

            if ( sl_Input.at( i_first ).section( "\t", i_Elevation, i_Elevation ) != "0" )
            {
                if ( sl_Input.at( i_first ).section( "\t", i_Elevation, i_Elevation ).toDouble() > 50. )
                    s_Elevation1 = QString( "-%1" ).arg( sl_Input.at( i_first ).section( "\t", i_Elevation, i_Elevation ).toDouble(), 0, 'f', 0 );
                else
                    s_Elevation1 = QString( "-%1" ).arg( sl_Input.at( i_first ).section( "\t", i_Elevation, i_Elevation ).toDouble(), 0, 'f', 1 );
            }

            s_Elevation2 = "";

            if ( sl_Input.at( i_last ).section( "\t", i_Elevation, i_Elevation ) != "0" )
            {
                if ( sl_Input.at( i_last ).section( "\t", i_Elevation, i_Elevation ).toDouble() > 50. )
                    s_Elevation2 = QString( "-%1" ).arg( sl_Input.at( i_last ).section( "\t", i_Elevation, i_Elevation ).toDouble(), 0, 'f', 0 );
                else
                    s_Elevation2 = QString( "-%1" ).arg( sl_Input.at( i_last ).section( "\t", i_Elevation, i_Elevation ).toDouble(), 0, 'f', 1 );
            }

            s_Device = sl_Input.at( i_first ).section( "\t", i_DeviceAbbr, i_DeviceAbbr );

            s_Device.replace( "CTDOZE", "CTD-RO" );
            s_Device.replace( "UCTD", "CTD-UW" );
            s_Device.replace( "MN_S5", "MSN" );
            s_Device.replace( "FLOAT", "ARGOFL" );
            s_Device.replace( "ADCP_150", "VMADCP-150" );
            s_Device.replace( "TSG_KEEL", "TSG" );
            s_Device.replace( "PCO2_GO", "UWPCO2" );
            s_Device.replace( "PCO2_SUB", "UWPCO2" );
            s_Device.replace( "WST", "SWEAS" );
            s_Device.replace( "OCEANET", "UWAOBS" );
            s_Device.replace( "SPM", "SPEC" );

            if ( i_first == i_last )
            {
                tout << s_CampaignLabel + "\t";
                tout << s_EventLabel << "\t" << s_Device << "\t";
                tout << sl_Input.at( i_first ).section( "\t", i_DeviceLong, i_DeviceLong ) << "\t" << sl_Input.at( i_first ).section( "\t", i_Action, i_Action ) << "\t";
                tout << s_DateTime1 << "\t" << s_Latitude1 << "\t" << s_Longitude1 << "\t" << s_Elevation1 << s_EOL;
            }
            else
            {
                tout << s_CampaignLabel + "\t";
                tout << s_EventLabel << "\t" << s_Device << "\t";
                tout << sl_Input.at( i_first ).section( "\t", i_DeviceLong, i_DeviceLong ) << "\t" << sl_Input.at( i_first ).section( "\t", i_Action, i_Action ) << "\t";
                tout << s_DateTime1 << "\t" << s_Latitude1 << "\t" << s_Longitude1 << "\t" << s_Elevation1 << "\t";
                tout << sl_Input.at( i_last ).section( "\t", i_Action, i_Action ) << "\t";
                tout << s_DateTime2 << "\t" << s_Latitude2 << "\t" << s_Longitude2 << "\t" << s_Elevation2 << s_EOL;
            }

            i_first = i;
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
// 2017-08-29

void MainWindow::doConvertDShipActionLogCruiseReport()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Convert DShip ActionLog..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = convertDShipActionLogCruiseReport( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gsl_FilenameList.count() );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting DShip ActionLog was canceled" ) );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2017-04-19

void MainWindow::doConvertDShipActionLog()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Convert DShip ActionLog..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = convertDShipActionLog( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gsl_FilenameList.count() );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting DShip ActionLog was canceled" ) );

    onError( err );
}

