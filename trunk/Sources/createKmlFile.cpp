/* 2010-08-15                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::createKmlFile( const QString &s_FilenameIn,
                               int &i_IconColor, int &i_IconSize,
                               int &i_TracklineColor, int &i_TracklineWidth,
                               QString &s_FilenameGoogleEarthProgram, bool &b_startGoogleEarth,
                               QString &s_FilenameOut, const int i_NumOfFiles )
{
    int         i               = 0;
    int         n               = 0;

    int			stopProgress	= 0;

    int         i_ID            = 0;
    int         i_StartID       = 0;
    int         i_EndID         = 0;

    double      d_Distance      = 0.0;
    double      d_Speed         = 0.0;
    double      d_Latitude      = 0.0;
    double      d_Longitude     = 0.0;
    double      d_Latitude_old  = 0.0;
    double      d_Longitude_old = 0.0;
    double      d_Seconds_old   = 0.0;
    double      d_PlotDistance  = 100.0;

    QString     s_Date          = "";
    QString     s_Time          = "";
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

    if ( doGPStoKmlOptionsDialog( i_StartID, i_EndID, i_IconColor, i_IconSize, i_TracklineColor, i_TracklineWidth, s_TrackName, s_FilenameGoogleEarthProgram, b_startGoogleEarth ) == QDialog::Rejected )
        return( _APPBREAK_ );

// **********************************************************************************************
// open output file

    s_FilenameOut = fi.absolutePath() + "/" + s_TrackName + ".kml";

    QFile fout( s_FilenameOut );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text) == false )
    {
        s_FilenameOut = "";
        return( -20 );
    }

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Creating KML file..." ), 2*sl_Input.count() );

// **********************************************************************************************

    QTextStream tout( &fout );
    tout.setCodec( "UTF-8" );

    tout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    tout << "<kml xmlns=\"http://earth.google.com/kml/2.1\">" << endl;
    tout << "<Document>" << endl;
/*
    tout << "  <ScreenOverlay>" << endl;
    tout << "    <name>Pan2Applic logo</name>" << endl;
    tout << "    <Icon><href>http://epic.awi.de/40953/121/Pan2Applic_logo.png</href></Icon>" << endl;
    tout << "    <overlayXY x=\"0\" y=\"1\" xunits=\"fraction\" yunits=\"fraction\"/>" << endl;
    tout << "    <screenXY x=\"5\" y=\"5\" xunits=\"pixels\" yunits=\"insetPixels\"/>" << endl;
    tout << "    <size x=\"200\" y=\"88\" xunits=\"pixel\" yunits=\"pixel\"/>" << endl;
    tout << "  </ScreenOverlay>" << endl;
*/
    tout << "  <Style id=\"circle-blue\"><IconStyle><Icon><href>http://epic.awi.de/40953/1/circle-blue.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"circle-green\"><IconStyle><Icon><href>http://epic.awi.de/40953/2/circle-green.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"circle-orange\"><IconStyle><Icon><href>http://epic.awi.de/40953/3/circle-orange.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"circle-red\"><IconStyle><Icon><href>http://epic.awi.de/40953/4/circle-red.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"circle-white\"><IconStyle><Icon><href>http://epic.awi.de/40953/5/circle-white.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"circle-yellow\"><IconStyle><Icon><href>http://epic.awi.de/40953/6/circle-yellow.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"square-blue\"><IconStyle><Icon><href>http://epic.awi.de/40953/7/square-blue.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"square-green\"><IconStyle><Icon><href>http://epic.awi.de/40953/8/square-green.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"square-orange\"><IconStyle><Icon><href>http://epic.awi.de/40953/9/square-orange.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"square-red\"><IconStyle><Icon><href>http://epic.awi.de/40953/10/square-red.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"square-white\"><IconStyle><Icon><href>http://epic.awi.de/40953/11/square-white.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"square-yellow\"><IconStyle><Icon><href>http://epic.awi.de/40953/12/square-yellow.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"star-blue\"><IconStyle><Icon><href>http://epic.awi.de/40953/13/star-blue.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"star-green\"><IconStyle><Icon><href>http://epic.awi.de/40953/14/star-green.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"star-orange\"><IconStyle><Icon><href>http://epic.awi.de/40953/15/star-orange.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"star-red\"><IconStyle><Icon><href>http://epic.awi.de/40953/16/star-red.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"star-white\"><IconStyle><Icon><href>http://epic.awi.de/40953/17/star-white.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"star-yellow\"><IconStyle><Icon><href>http://epic.awi.de/40953/18/star-yellow.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"triangle-blue\"><IconStyle><Icon><href>http://epic.awi.de/40953/19/triangle-blue.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"triangle-green\"><IconStyle><Icon><href>http://epic.awi.de/40953/20/triangle-green.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"triangle-orange\"><IconStyle><Icon><href>http://epic.awi.de/40953/21/triangle-orange.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"triangle-red\"><IconStyle><Icon><href>http://epic.awi.de/40953/22/triangle-red.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"triangle-white\"><IconStyle><Icon><href>http://epic.awi.de/40953/23/triangle-white.png</href></Icon></IconStyle></Style>" << endl;
    tout << "  <Style id=\"triangle-yellow\"><IconStyle><Icon><href>http://epic.awi.de/40953/24/triangle-yellow.png</href></Icon></IconStyle></Style>" << endl;

// **********************************************************************************************
// Way points

    tout << "<Folder>" << endl;
    tout << "<name>Point Set</name>" << endl;
    tout << "<open>0</open>" << endl;

    while ( ( i<n ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( sl_Input.at( i ).section( ",", 1, 1 ) == "T" )
        {
            i_ID = sl_Input.at( i ).section( ",", 0, 0 ).toInt();

            if ( ( i_StartID <= i_ID ) && ( i_ID <= i_EndID ) )
            {
                s_Time      = sl_Input.at( i ).section( ",", 3, 3 ).left( 2 ) + ":" + sl_Input.at( i ).section( ",", 3, 3 ).mid( 2, 2 ) + ":" + sl_Input.at( i ).section( ",", 3, 3 ).mid( 4, 2 );
                s_Date      = "20" + sl_Input.at( i ).section( ",", 2, 2 ).left( 2 ) + "-" + sl_Input.at( i ).section( ",", 2, 2 ).mid( 2, 2 ) + "-" + sl_Input.at( i ).section( ",", 2, 2 ).mid( 4, 2 );

                s_Latitude  = extractLatitude( sl_Input.at(i) );
                s_Longitude = extractLongitude( sl_Input.at(i) );

                d_Latitude  = s_Latitude.toDouble();
                d_Longitude = s_Longitude.toDouble();

                d_Distance  = calculateDistance( d_Latitude, d_Longitude, d_Latitude_old, d_Longitude_old ); // [m]

                if ( ( i_ID == i_StartID ) || ( i_ID == i_EndID ) || ( d_Distance > d_PlotDistance ) )
                {
                    d_Speed = 3.6 * d_Distance/( (double) i - d_Seconds_old ); // [m/s] -> [km/h]

                    tout << "<Placemark>";
//                  tout << "<name>" << s_Time << "</name>";

                    switch ( i_IconColor )
                    {
                        case _RED_:
                            tout << "<styleUrl>#red</styleUrl>";
                            break;
                        case _GREEN_:
                            tout << "<styleUrl>#green</styleUrl>";
                            break;
                        case _BLUE_:
                            tout << "<styleUrl>#blue</styleUrl>";
                            break;
                        case _ORANGE_:
                            tout << "<styleUrl>#orange</styleUrl>";
                            break;
                        case _YELLOW_:
                            tout << "<styleUrl>#yellow</styleUrl>";
                            break;
                        default:
                            tout << "<styleUrl>#red</styleUrl>";
                            break;
                    }

                    tout << "<Style><IconStyle>" << QString( "<scale>%1</scale>" ).arg( (float) i_IconSize/100. ) << "</IconStyle></Style>";
                    tout << "<Point><coordinates>" << s_Longitude << "," << s_Latitude << "</coordinates></Point>";
                    tout << "<description>" << endl;
                    tout << "<![CDATA[Time: " << s_Time << "<br />Date: " << s_Date << "<br />Latitude: " << d_Latitude << "° N<br />Longitude: " << d_Longitude << "° E<br />Altitude: " << sl_Input.at( i ).section( ",", 6, 6 ).simplified() << " m<br />Speed: " << d_Speed << " km/h<br />ID: " << i_ID << "<br />]]>" << endl;
                    tout << "</description>" << endl;
                    tout << "</Placemark>" << endl;

                    d_Latitude_old  = d_Latitude;
                    d_Longitude_old = d_Longitude;
                    d_Seconds_old   = (double) i;

                    if ( d_Speed < 50. )
                        d_PlotDistance = 100;
                    else
                        d_PlotDistance = 1000;
                }
            }
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    tout << "</Folder>" << endl;

// **********************************************************************************************
// Track line

    tout << "<Placemark><name>Trackline</name>" << endl;

    switch ( i_TracklineColor )
    {
        case _RED_:
            tout << "<Style><LineStyle><color>ff0000ff</color>";
            break;
        case _GREEN_:
            tout << "<Style><LineStyle><color>ff00ff00</color>";
            break;
        case _BLUE_:
            tout << "<Style><LineStyle><color>ffff0000</color>";
            break;
        case _ORANGE_:
            tout << "<Style><LineStyle><color>ff0099ff</color>";
            break;
        case _YELLOW_:
            tout << "<Style><LineStyle><color>ff33ffff</color>";
            break;
        default:
            tout << "<Style><LineStyle><color>ff0099ff</color>";
            break;
    }

    tout << "<width>" << (float) i_TracklineWidth/10. << "</width></LineStyle></Style>" << endl;

    tout << "<LineString>" << endl;
    tout << "<tessellate>1</tessellate>" << endl;
    tout << "<coordinates>" << endl;

    i = 1;

    while ( ( i<n ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( sl_Input.at( i ).section( ",", 1, 1 ) == "T" )
        {
            i_ID = sl_Input.at( i ).section( ",", 0, 0 ).toInt();

            if ( ( i_StartID <= i_ID ) && ( i_ID <= i_EndID ) )
            {
                s_Latitude  = extractLatitude( sl_Input.at(i) );
                s_Longitude = extractLongitude( sl_Input.at(i) );

                tout << s_Longitude << "," << s_Latitude << "," << sl_Input.at( i ).section( ",", 6, 6 ).simplified() << endl;
            }
        }

        stopProgress = incProgress( i_NumOfFiles, ++i+n );
    }

    tout << "</coordinates>" << endl;
    tout << "</LineString>" << endl;
    tout << "</Placemark>" << endl;
    tout << "</Document>" << endl;
    tout << "</kml>" << endl;

    fout.close();

    resetProgress( i_NumOfFiles );

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2010-08-16

double MainWindow::calculateDistance( const double Latitude1, const double Longitude1, const double Latitude2, const double Longitude2)
{
    double pi180 = 0.0174532925199433;

    if ( ( Latitude2 == 0.0 ) && ( Longitude2 == 0.0 ) )
        return ( 0.0 );

    double Distance = (acos(sin(Latitude1*pi180)*sin(Latitude2*pi180)+cos(Latitude1*pi180)*cos(Latitude2*pi180)*cos(Longitude1*pi180-Longitude2*pi180) )*6366707);

    return ( Distance );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doCreateKmlFile()
{
    int         i				= 0;
    int         err				= 0;
    int         stopProgress	= 0;

    QString     s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Creating KML file..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            err = createKmlFile( gsl_FilenameList.at( i ), gi_IconColor, gi_IconSize, gi_TracklineColor, gi_TracklineWidth, gs_FilenameGoogleEarthProgram, gb_startGoogleEarth, s_FilenameOut, gsl_FilenameList.count() );

            stopProgress = incFileProgress( gsl_FilenameList.count(), ++i );
        }

        resetFileProgress( gsl_FilenameList.count() );
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Creating KML file  was canceled" ), false, false );

// **********************************************************************************************
// start Google Earth

    if ( ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) && ( gb_startGoogleEarth == true ) )
        err = startGoogleEarth( gs_FilenameGoogleEarthProgram, s_FilenameOut );

    onError( err );
}

