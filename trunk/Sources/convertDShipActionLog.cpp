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
    int         k               = 1;
    int         n               = 0;

    int			stopProgress	= 0;

    int         i_Format        = -999;

    int			i_ColumnCampaign         = 0;
    int			i_ColumnEventLabel       = 0;
    int			i_ColumnDateTime         = 1;
    int			i_ColumnDeviceLong       = 2;
    int			i_ColumnDeviceAbbr       = 3;
    int			i_ColumnAction           = 4;
    int			i_ColumnLatitude         = 9;
    int			i_ColumnLongitude        = 10;
    int			i_ColumnDepth            = 11;

    QString     s_CampaignLabel          = "";
    QString     s_EventLabel             = "";
    QString     s_DeviceLong             = "";
    QString     s_DeviceAbbr             = "";

    QString     s_StationNumber          = "";
    QString     s_DeviceDeploymentNumber = "";

    QString     s_EOL                    = setEOLChar( i_EOL );

    QStringList sl_Input;
    QStringList sl_DeviceLong;

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( -10 );

// **********************************************************************************************
// check format

    if ( ( sl_Input.at( 0 ).section( "\t", 9, 9 ).startsWith( "Latitude" ) == true ) && ( sl_Input.at( 0 ).section( "\t", 10, 10 ).startsWith( "Longitude" ) == true ) ) // BSH
    {
        i_Format           = _BSH_;
        i_ColumnLatitude   = 9;
        i_ColumnLongitude  = 10;
    }

    if ( ( sl_Input.at( 0 ).section( "\t", 14, 14 ).startsWith( "Latitude" ) == true ) && ( sl_Input.at( 0 ).section( "\t", 15, 15 ).startsWith( "Longitude" ) == true ) ) // AWI
    {
        i_Format           = _AWI_;
        i_ColumnLatitude   = 14;
        i_ColumnLongitude  = 15;
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

    tout << "Station" << "\t" << "Date" << "\t" << "Time" << "\t";
    tout << "Latitude" << "\t" << "Longitude" << "\t" << "Depth (m)" << "\t";
    tout << "Gear" << "\t" << "Action" << "\t" << "Comment" << "\t";
    tout << "sort 1" << "\t" << "sort 2" << "\t" << "sort 3" << "\t" << "sort 4";
    tout << s_EOL;

// **********************************************************************************************

    sl_DeviceLong.append( "Campaign\tGear abbreviations\tGear\tComment" );

// **********************************************************************************************

    while ( ( i<sl_Input.count() ) && ( stopProgress != _APPBREAK_ ) )
    {
        int i_StationStart  = 0;
        int i_StationEnd    = 0;
        int i_ProfileStart  = 0;
        int i_ProfileEnd    = 0;
        int i_AtDepth       = 0;

        QStringList sl_DateTime;
        QStringList sl_Latitude;
        QStringList sl_Longitude;
        QStringList sl_Depth;
        QStringList sl_Action;

        s_CampaignLabel = sl_Input.at( i ).section( "\t", i_ColumnCampaign, i_ColumnCampaign ).section( "_", 0, 0 );
        s_DeviceLong    = sl_Input.at( i ).section( "\t", i_ColumnDeviceLong, i_ColumnDeviceLong );
        s_DeviceAbbr    = sl_Input.at( i ).section( "\t", i_ColumnDeviceAbbr, i_ColumnDeviceAbbr );
        s_EventLabel    = sl_Input.at( i ).section( "\t", i_ColumnEventLabel, i_ColumnEventLabel ).replace( "_00_", "_0_" );
        s_StationNumber = s_EventLabel.section( "_", 1, 1 ).section( "-", 0, 0 );

        sl_DeviceLong.append( s_CampaignLabel + "\t" + s_DeviceAbbr + "\t" + s_DeviceLong );

        if ( s_EventLabel.contains( "underway", Qt::CaseInsensitive ) == false )
            s_DeviceDeploymentNumber = s_EventLabel.section( "_", 1, 1 ).section( "-", 1, 1 );
        else
            s_DeviceDeploymentNumber = s_EventLabel.section( "_", 2, 2 ).section( "-", 1, 1 );

        if ( sl_Input.at( i ).contains( "station start", Qt::CaseInsensitive ) == true )
            i_StationStart = i;

        if ( sl_Input.at( i ).contains( "in the water", Qt::CaseInsensitive ) == true )
            i_StationStart = i;

        if ( sl_Input.at( i ).contains( "profile start", Qt::CaseInsensitive ) == true )
            i_ProfileStart = i;

        if ( sl_Input.at( i ).contains( "max depth/on ground", Qt::CaseInsensitive ) == true )
            i_AtDepth = i;

        if ( sl_Input.at( i ).contains( "profile end", Qt::CaseInsensitive ) == true )
            i_ProfileEnd = i;

        if ( sl_Input.at( i ).contains( "on deck", Qt::CaseInsensitive ) == true )
            i_StationEnd = i;

        if ( sl_Input.at( i ).contains( "station end", Qt::CaseInsensitive ) == true )
            i_StationEnd = i;

        if ( i_StationStart > 0 )
        {
            sl_DateTime.append( getDateTime( sl_Input.at( i_StationStart ), i_ColumnDateTime ) );
            sl_Latitude.append( getLatitude( sl_Input.at( i_StationStart ), i_ColumnLatitude, i_ColumnLongitude, i_Format ) );
            sl_Longitude.append( getLongitude( sl_Input.at( i_StationStart ), i_ColumnLatitude, i_ColumnLongitude, i_Format ) );
            sl_Depth.append( getDepth( sl_Input.at( i_StationStart ), i_ColumnDepth ) );
            sl_Action.append( getAction( sl_Input.at( i_StationStart ), i_ColumnAction ) );
        }
        else
        {
            sl_DateTime.append( "NA" );
            sl_Latitude.append( "NA" );
            sl_Longitude.append( "NA" );
            sl_Depth.append( "NA" );
            sl_Action.append( "NA" );
        }

        if ( i_ProfileStart > 0 )
        {
            sl_DateTime.append( getDateTime( sl_Input.at( i_ProfileStart ), i_ColumnDateTime ) );
            sl_Latitude.append( getLatitude( sl_Input.at( i_ProfileStart ), i_ColumnLatitude, i_ColumnLongitude, i_Format ) );
            sl_Longitude.append( getLongitude( sl_Input.at( i_ProfileStart ), i_ColumnLatitude, i_ColumnLongitude, i_Format ) );
            sl_Depth.append( getDepth( sl_Input.at( i_ProfileStart ), i_ColumnDepth ) );
            sl_Action.append( getAction( sl_Input.at( i_ProfileStart ), i_ColumnAction ) );
        }
        else
        {
            sl_DateTime.append( "NA" );
            sl_Latitude.append( "NA" );
            sl_Longitude.append( "NA" );
            sl_Depth.append( "NA" );
            sl_Action.append( "NA" );
        }

        if ( i_AtDepth > 0 )
        {
            sl_DateTime.append( getDateTime( sl_Input.at( i_AtDepth ), i_ColumnDateTime ) );
            sl_Latitude.append( getLatitude( sl_Input.at( i_AtDepth ), i_ColumnLatitude, i_ColumnLongitude, i_Format ) );
            sl_Longitude.append( getLongitude( sl_Input.at( i_AtDepth ), i_ColumnLatitude, i_ColumnLongitude, i_Format ) );
            sl_Depth.append( getDepth( sl_Input.at( i_AtDepth ), i_ColumnDepth ) );
            sl_Action.append( getAction( sl_Input.at( i_AtDepth ), i_ColumnAction ) );
        }
        else
        {
            sl_DateTime.append( "NA" );
            sl_Latitude.append( "NA" );
            sl_Longitude.append( "NA" );
            sl_Depth.append( "NA" );
            sl_Action.append( "NA" );
        }

        if ( i_ProfileEnd > 0 )
        {
            sl_DateTime.append( getDateTime( sl_Input.at( i_ProfileEnd ), i_ColumnDateTime ) );
            sl_Latitude.append( getLatitude( sl_Input.at( i_ProfileEnd ), i_ColumnLatitude, i_ColumnLongitude, i_Format ) );
            sl_Longitude.append( getLongitude( sl_Input.at( i_ProfileEnd ), i_ColumnLatitude, i_ColumnLongitude, i_Format ) );
            sl_Depth.append( getDepth( sl_Input.at( i_ProfileEnd ), i_ColumnDepth ) );
            sl_Action.append( getAction( sl_Input.at( i_ProfileEnd ), i_ColumnAction ) );
        }
        else
        {
            sl_DateTime.append( "NA" );
            sl_Latitude.append( "NA" );
            sl_Longitude.append( "NA" );
            sl_Depth.append( "NA" );
            sl_Action.append( "NA" );
        }

        if ( i_StationEnd > 0 )
        {
            sl_DateTime.append( getDateTime( sl_Input.at( i_StationEnd ), i_ColumnDateTime ) );
            sl_Latitude.append( getLatitude( sl_Input.at( i_StationEnd ), i_ColumnLatitude, i_ColumnLongitude, i_Format ) );
            sl_Longitude.append( getLongitude( sl_Input.at( i_StationEnd ), i_ColumnLatitude, i_ColumnLongitude, i_Format ) );
            sl_Depth.append( getDepth( sl_Input.at( i_StationEnd ), i_ColumnDepth ) );
            sl_Action.append( getAction( sl_Input.at( i_StationEnd ), i_ColumnAction ) );
        }
        else
        {
            sl_DateTime.append( "NA" );
            sl_Latitude.append( "NA" );
            sl_Longitude.append( "NA" );
            sl_Depth.append( "NA" );
            sl_Action.append( "NA" );
        }

        for ( int j=0; j<sl_DateTime.count(); j++ )
        {
            if ( sl_DateTime.at( j ) != "NA" )
            {
                tout << s_EventLabel << "\t" << sl_DateTime.at( j ).section( "T", 0, 0 ) << "\t" << sl_DateTime.at( j ).section( "T", 1, 1 ) << "\t";
                tout << sl_Latitude.at( j ) << "\t" << sl_Longitude.at( j ) << "\t" << sl_Depth.at( j ) << "\t";
                tout << s_DeviceAbbr << "\t" << sl_Action.at( j ) << "\t";
                tout << QString( "=WENN(UND(A%1=A%2;H%1=H%2);\"<==\";\"\")" ).arg( k ).arg( k+1 ) << "\t"; k++;
                tout << s_CampaignLabel << "\t" << s_StationNumber << "\t" << s_DeviceDeploymentNumber << "\t" << i;
                tout << s_EOL;
            }
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

// **********************************************************************************************

    sl_DeviceLong.sort();

    tout << s_EOL << sl_DeviceLong.at( 0 ) << s_EOL;

    for ( int j=1; j<sl_DeviceLong.count(); j++ )
    {
        if ( sl_DeviceLong.at( j ) != sl_DeviceLong.at( j-1 ) )
            tout << sl_DeviceLong.at( j ) << s_EOL;
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
// 2017-09-01

QString MainWindow::getDateTime( const QString s_Input, const int i_ColumnDateTime )
{
    return( s_Input.section( "\t", i_ColumnDateTime, i_ColumnDateTime ).left( 16 ).replace( "/", "-" ).replace( " ", "T" ) );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2017-09-01

QString MainWindow::getLatitude( const QString s_Input, const int i_ColumnLatitude, const int i_ColumnLongitude, const int i_Format )
{
    convertPosition *llc = new convertPosition( 5, _DOT_ );

    switch ( i_Format )
    {
    case _AWI_:
        return( QString( "%1" ).arg( s_Input.section( "\t", i_ColumnLatitude, i_ColumnLatitude ).toDouble(), 0, 'f', 5 ) );
        break;

    case _BSH_:
        llc->convertLLtoLL( s_Input.section( "\t", i_ColumnLatitude, i_ColumnLatitude ), s_Input.section( "\t", i_ColumnLongitude, i_ColumnLongitude ) );
        return( llc->Latitude() );
    }

    return( "NA" );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2017-09-01

QString MainWindow::getLongitude( const QString s_Input, const int i_ColumnLatitude, const int i_ColumnLongitude, const int i_Format )
{
    convertPosition *llc = new convertPosition( 5, _DOT_ );

    switch ( i_Format )
    {
    case _AWI_:
        return( QString( "%1" ).arg( s_Input.section( "\t", i_ColumnLongitude, i_ColumnLongitude ).toDouble(), 0, 'f', 5 ) );
        break;

    case _BSH_:
        llc->convertLLtoLL( s_Input.section( "\t", i_ColumnLatitude, i_ColumnLatitude ), s_Input.section( "\t", i_ColumnLongitude, i_ColumnLongitude ) );
        return( llc->Longitude() );
    }

    return( "NA" );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2017-09-01

QString MainWindow::getDepth( const QString s_Input, const int i_ColumnDepth )
{
    if ( s_Input.section( "\t", i_ColumnDepth, i_ColumnDepth ).toDouble() != 0. )
    {
        if ( s_Input.section( "\t", i_ColumnDepth, i_ColumnDepth ).toDouble() > 50. )
            return( QString( "%1" ).arg( s_Input.section( "\t", i_ColumnDepth, i_ColumnDepth ).toDouble(), 0, 'f', 0 ) );
        else
            return( QString( "%1" ).arg( s_Input.section( "\t", i_ColumnDepth, i_ColumnDepth ).toDouble(), 0, 'f', 1 ) );
    }

    return( "NA" );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2017-09-01

QString MainWindow::getAction( const QString s_Input, const int i_ColumnAction )
{
    QString s_Action = s_Input.section( "\t", i_ColumnAction, i_ColumnAction );

    if ( s_Action.contains( "in the water", Qt::CaseInsensitive ) == true )
        return ( "station start" );

    if ( s_Action.contains( "max depth/on ground", Qt::CaseInsensitive ) == true )
        return ( "at depth" );

    if ( s_Action.contains( "on deck", Qt::CaseInsensitive ) == true )
        return ( "station end" );

    return ( s_Action );
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
    tout << "Action2" << "\t" << "DateTimeEvent2" << "\t" << "LatitudeEvent2" << "\t" << "LongitudeEvent2" << "\t" << "ElevationEvent2" << "\t";
    tout << "Site" << "\t" << "Event" << "\t" << "No" << s_EOL;

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
                if ( sl_Input.at( j ).contains( "station start", Qt::CaseInsensitive ) == true )
                    i_first = j;

                if ( sl_Input.at( j ).contains( "profile start", Qt::CaseInsensitive ) == true )
                    i_first = j;

                if ( sl_Input.at( j ).contains( "station end", Qt::CaseInsensitive ) == true )
                    i_last = j;

                if ( sl_Input.at( j ).contains( "profile end", Qt::CaseInsensitive ) == true )
                    i_last = j;

                if ( sl_Input.at( j ).contains( "max depth/on ground", Qt::CaseInsensitive ) == true )
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
                tout << s_DateTime1 << "\t" << s_Latitude1 << "\t" << s_Longitude1 << "\t" << s_Elevation1 << "\t\t\t\t\t\t";
                tout << s_EventLabel.section( "_", 1, 1 ).section( "-", 0, 0 ) << "\t" << s_EventLabel.section( "-", 1, 1 ) << s_EOL;
            }
            else
            {
                tout << s_CampaignLabel + "\t";
                tout << s_EventLabel << "\t" << s_Device << "\t";
                tout << sl_Input.at( i_first ).section( "\t", i_DeviceLong, i_DeviceLong ) << "\t" << sl_Input.at( i_first ).section( "\t", i_Action, i_Action ) << "\t";
                tout << s_DateTime1 << "\t" << s_Latitude1 << "\t" << s_Longitude1 << "\t" << s_Elevation1 << "\t";
                tout << sl_Input.at( i_last ).section( "\t", i_Action, i_Action ) << "\t";
                tout << s_DateTime2 << "\t" << s_Latitude2 << "\t" << s_Longitude2 << "\t" << s_Elevation2 << "\t";
                tout << s_EventLabel.section( "_", 1, 1 ).section( "-", 0, 0 ) << "\t" << s_EventLabel.section( "-", 1, 1 ) << s_EOL;
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

