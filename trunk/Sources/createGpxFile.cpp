/* 2008-07-23                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::createGpxFile( const QString &s_FilenameIn, const int i_NumOfFiles )
{
    int         i               = 0;
    int         k               = 0;
    int         n               = 0;

    int			stopProgress	= 0;

    int         i_ID            = 0;
    int         i_StartID       = 0;
    int         i_EndID         = 0;

    float       f_minlat        =  999.;
    float       f_maxlat        = -999.;
    float       f_minlong       =  999.;
    float       f_maxlong       = -999.;

    QString     s_DateTime      = "";
    QString     s_Latitude      = "";
    QString     s_Longitude     = "";
    QString     s_TrackName     = "";

    QStringList sl_Input;

    QFileInfo fi( s_FilenameIn );

// **********************************************************************************************
// read file

    QFile fin( s_FilenameIn );

    if ( fin.open( QIODevice::ReadOnly | QIODevice::Text ) == false )
        return( -10 );

    QTextStream tin( &fin );

    while ( tin.atEnd() == false )
        sl_Input.append( tin.readLine().replace( QChar( 0x00 ), "" ) );

    fin.close();

    n = sl_Input.count();

// **********************************************************************************************

    if ( ( n<2 ) || ( sl_Input.at( i++ ) != "INDEX,TAG,DATE,TIME,LATITUDE N/S,LONGITUDE E/W,HEIGHT,SPEED,HEADING,VOX" ) )
        return( _APPBREAK_ );

    i_StartID   = sl_Input.at( 1 ).section( ",", 0, 0 ).toInt();
    i_EndID     = sl_Input.at( sl_Input.count() - 1 ).section( ",", 0, 0 ).toInt();
    s_TrackName = fi.baseName();

    if ( doGPStoGpxOptionsDialog( i_StartID, i_EndID, s_TrackName ) == QDialog::Rejected )
        return( _APPBREAK_ );

    for ( int i=1; i<n; i++ )
    {
        s_Latitude  = extractLatitude( sl_Input.at(i) );
        s_Longitude = extractLongitude( sl_Input.at(i) );

        f_minlat    = qMin( f_minlat, s_Latitude.toFloat() );
        f_maxlat    = qMax( f_maxlat, s_Latitude.toFloat() );
        f_minlong   = qMin( f_minlong, s_Longitude.toFloat() );
        f_maxlong   = qMax( f_maxlong, s_Longitude.toFloat() );
    }

// **********************************************************************************************
// open output file

    QFile fout( fi.absolutePath() + "/" + s_TrackName + ".gpx" );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
        return( -20 );

    QTextStream tout( &fout ); tout.setCodec( "UTF-8" );

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Creating GPX file..." ), 2*sl_Input.count() );

// **********************************************************************************************

    tout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    tout << "<gpx" << endl;
    tout << "version=\"1.1\"" << endl;
    tout << "creator=\"PanConverter - https://pangaea.de/\"" << endl;
    tout << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" << endl;
    tout << "xmlns=\"http://www.topografix.com/GPX/1/1\"" << endl;
    tout << "xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\">" << endl;

    tout << "<time>" << QDateTime::currentDateTime().toString( "yyyy-MM-ddThh:mm:ss" ) << "</time>" << endl;
    tout << QString( "<bounds minlat=\"%1\" minlon=\"%2\" maxlat=\"%3\" maxlon=\"%4\"/>" ).arg( f_minlat ).arg( f_minlong ).arg( f_maxlat ).arg( f_maxlong ) << endl;

// **********************************************************************************************
// Way points

    while ( ( i<n ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( sl_Input.at( i ).section( ",", 1, 1 ) == "C" )
        {
            i_ID = sl_Input.at( i ).section( ",", 0, 0 ).toInt();

            if ( ( i_StartID <= i_ID ) && ( i_ID <= i_EndID ) )
            {
                s_DateTime  = "20" + sl_Input.at( i ).section( ",", 2, 2 ).left( 2 ) + "-" + sl_Input.at( i ).section( ",", 2, 2 ).mid( 2, 2 ) + "-" + sl_Input.at( i ).section( ",", 2, 2 ).mid( 4, 2 ) + "T" + sl_Input.at( i ).section( ",", 3, 3 ).left( 2 ) + ":" + sl_Input.at( i ).section( ",", 3, 3 ).mid( 2, 2 ) + ":" + sl_Input.at( i ).section( ",", 3, 3 ).mid( 4, 2 );

                s_Latitude  = extractLatitude( sl_Input.at(i) );
                s_Longitude = extractLongitude( sl_Input.at(i) );

                tout << "<wpt lat=\"" << s_Latitude << "\" lon=\"" << s_Longitude << "\">" << endl;
                tout << "  <ele>" << sl_Input.at( i ).section( ",", 6, 6 ).simplified() << "</ele>" << endl;
                tout << "  <time>" << s_DateTime << "</time>" << endl;
                tout << "  <name>W" << ++k << "</name>" << endl;
                tout << "  <sym>Dot</sym>" << endl;
                tout << "</wpt>" << endl;
            }
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

// **********************************************************************************************
// Track points

    tout << "<trk>" << endl;
    tout << "<name>" << s_TrackName << "</name>" << endl;
    tout << "<trkseg>" << endl;

    i = 1;

    while ( ( i<n ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( sl_Input.at( i ).section( ",", 1, 1 ) == "T" )
        {
            i_ID = sl_Input.at( i ).section( ",", 0, 0 ).toInt();

            if ( ( i_StartID <= i_ID ) && ( i_ID <= i_EndID ) )
            {
                s_DateTime  = "20" + sl_Input.at( i ).section( ",", 2, 2 ).left( 2 ) + "-" + sl_Input.at( i ).section( ",", 2, 2 ).mid( 2, 2 ) + "-" + sl_Input.at( i ).section( ",", 2, 2 ).mid( 4, 2 ) + "T" + sl_Input.at( i ).section( ",", 3, 3 ).left( 2 ) + ":" + sl_Input.at( i ).section( ",", 3, 3 ).mid( 2, 2 ) + ":" + sl_Input.at( i ).section( ",", 3, 3 ).mid( 4, 2 );

                s_Latitude  = extractLatitude( sl_Input.at(i) );
                s_Longitude = extractLongitude( sl_Input.at(i) );

                tout << "<trkpt lat=\"" << s_Latitude << "\" lon=\"" << s_Longitude << "\">" << endl;
                tout << "  <ele>" << sl_Input.at( i ).section( ",", 6, 6 ).simplified() << "</ele>" << endl;
                tout << "  <time>" << s_DateTime << "</time>" << endl;
                tout << "</trkpt>" << endl;
            }
        }

        stopProgress = incProgress( i_NumOfFiles, ++i+n );
    }

    tout << "</trkseg>" << endl;
    tout << "</trk>" << endl;
    tout << "</gpx>" << endl;

    fout.close();

    resetProgress( i_NumOfFiles );

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2010-07-31

QString MainWindow::extractLatitude( const QString &InputStr )
{
    QString s_Latitude  = InputStr.section( ",", 4, 4 );

    s_Latitude.replace( "N", "" );

    if ( s_Latitude.endsWith( "S" ) == true )
    {
        s_Latitude.replace( "S", "" );
        s_Latitude = "-" + s_Latitude;
    }

    return( s_Latitude );
}


// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2010-07-31

QString MainWindow::extractLongitude( const QString &InputStr )
{
    QString s_Longitude  = InputStr.section( ",", 5, 5 );

    s_Longitude.replace( "E", "" );

    if ( s_Longitude.endsWith( "W" ) == true )
    {
        s_Longitude.replace( "W", "" );
        s_Longitude = "-" + s_Longitude;
    }

    return( s_Longitude );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doCreateGpxFile()
{
    int         i				= 0;
    int         err				= 0;
    int         stopProgress	= 0;

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Creating GPX file..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            err = createGpxFile( gsl_FilenameList.at( i ), gsl_FilenameList.count() );

            stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
        }

        resetFileProgress( gsl_FilenameList.count() );
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Creating GPX file  was canceled" ), false, false );

    onError( err );
}

