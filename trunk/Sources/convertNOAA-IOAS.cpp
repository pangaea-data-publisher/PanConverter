/* 2008-07-23                 */
/* Dr. Rainer Sieger          */

#include <QtGui>

#include "Application.h"
#include "convertPosition.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::convertNOAA_IOAS( const int i_Type, const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_NumOfFiles )
{
    int         i                   = 0;
    int         k                   = 0;
    int         n                   = 0;
    int         m                   = 0;
    int         s                   = 0;

    int         stopProgress        = 0;

    QString     s_EOL               = setEOLChar( i_EOL );

    QString     s_Out               = "";
    QString     s_Header            = "";

    QString     s_Station           = "";
    QString     s_Latitude          = "";
    QString     s_Longitude         = "";
    QString     s_DateTime          = "";
    QString     s_BottomDepth       = "";
    QString     s_EventLabel        = "";
    QString     s_Metadata          = "";
    QString     s_Comment           = "";
    QString     s_Unit              = "";
    QString     s_Unit_requested    = "";
    QString     s_BaseDir           = "";

    QStringList sl_Input;

    QDateTime   dt;

    convertPosition *llc = new convertPosition( 4, _DOT_ );

    QFileInfo fi_in  = QFileInfo( s_FilenameIn );
    QFileInfo fi_out = QFileInfo( s_FilenameOut );

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting NOAA-IOAS data..." ), sl_Input.count() );

// **********************************************************************************************

    switch ( i_Type )
    {
    case _PLANKTON_:
        s_Unit_requested = " [*]";
        break;
    case _PHYTOPLANKTON1_:
        s_Unit_requested = " [#/L]";
        break;
    case _PHYTOPLANKTON2_:
        s_Unit_requested = " [*]";
        break;
    case _ZOOPLANKTON1_:
        s_Unit_requested = " [#/SAMPLE]";
        break;
    case _ZOOPLANKTON2_:
        s_Unit_requested = " [#/M3]";
        break;
    case _ZOOPLANKTON3_:
        s_Unit_requested = " [MG/SAMPLE]";
        break;
    case _ZOOPLANKTON4_:
        s_Unit_requested = " [*]";
        break;
    default:
        s_Unit_requested = "";
        break;
    }

// **********************************************************************************************

    while ( ( i<n ) && ( stopProgress != _APPBREAK_ ) )
    {
        if ( sl_Input.at( i ).startsWith( "STATION" ) == true )
        {
            s_EventLabel = fi_in.baseName() + tr( "-track" );
            s_Station    = sl_Input.at( i ).section( ",", 1, 1 );

            if ( sl_Input.at( i ).section( ",", 3, 3 ).isEmpty() == false )
                s_Station += tr( "-" ) + sl_Input.at( i ).section( ",", 3, 3 );

            i += 2;

            s_Latitude = sl_Input.at( i ).section( ",", 0, 0 ) + "°";

            if ( sl_Input.at( i ).section( ",", 1, 1 ).isEmpty() == false )
                s_Latitude.append( sl_Input.at( i ).section( ",", 1, 1 ) + "'" );

            if ( sl_Input.at( i ).section( ",", 2, 2 ).isEmpty() == false )
                s_Latitude.append( sl_Input.at( i ).section( ",", 2, 2 ) + "''" );

            s_Latitude.append( " " + sl_Input.at( i ).section( ",", 3, 3 ) );

            s_Longitude = sl_Input.at( i ).section( ",", 4, 4 ) + "°";

            if ( sl_Input.at( i ).section( ",", 1, 1 ).isEmpty() == false )
                s_Longitude.append( sl_Input.at( i ).section( ",", 5, 5 ) + "'" );

            if ( sl_Input.at( i ).section( ",", 2, 2 ).isEmpty() == false )
                s_Longitude.append( sl_Input.at( i ).section( ",", 6, 6 ) + "''" );

            s_Longitude.append( " " + sl_Input.at( i ).section( ",", 7, 7 ) );

            llc->convertLLtoLL( s_Latitude, s_Longitude );

            dt = QDateTime( QDate( sl_Input.at( i ).section( ",", 8, 8 ).toInt(), sl_Input.at( i ).section( ",", 9, 9 ).toInt(), sl_Input.at( i ).section( ",", 10, 10 ).toInt() ), QTime( sl_Input.at( i ).section( ",", 11, 11 ).toInt(), sl_Input.at( i ).section( ",", 12, 12 ).toInt() ) );

            if ( dt.isValid() == true )
                s_DateTime = dt.toString( "yyyy-MM-ddTHH:mm" );
            else
                s_DateTime = "not vaild";

            if ( sl_Input.at( i+3 ).startsWith( "BOTTOM DEPTH" ) == true )
                s_BottomDepth = sl_Input.at( i+3 ).section( ",", 1, 1 );
            else
                s_BottomDepth = "";
        }

        switch ( i_Type )
        {
        case _HYDROLOGY_:
            if ( sl_Input.at( i ).startsWith( "TYPE,HYDROLOGY" ) == true )
            {
                i++;

                s_Header = "";

                m = sl_Input.at( i ).count( "," ) + 1;

                for ( int j=0; j<m; j++ )
                {
                    if ( sl_Input.at( i ).section( ",", j, j ).isEmpty() == false )
                        s_Header += "\t" + sl_Input.at( i ).section( ",", j, j ) + tr( " [" ) + sl_Input.at( i+1 ).section( ",", j, j ) + tr( "]" );
                }

                s_Header.replace( "DEPTH [M]", "Depth, water [m]" );
                s_Header.replace( "TEMP [C]", "Temperature, water [°C]" );
                s_Header.replace( "SAL [PSU]", "Salinity []@PSU" );

                s_Header.replace( "OXY [ML/L]", "Oxygen [ml/l]" );
                s_Header.replace( "OXY [MG/L]", "Oxygen [mg/l]" );
                s_Header.replace( "OXY SATUR [PERCENT]", "Oxygen, saturation [%]" );

                s_Header.replace( "PO4 [UG-AT/L]", "Phosphate [µg-at/l]" );
                s_Header.replace( "NO2 [UG-AT/L]", "Nitrite [µg-at/l]" );
                s_Header.replace( "NO3 [UG-AT/L]", "Nitrate [µg-at/l]" );
                s_Header.replace( "SIO4 [UG-AT/L]", "Silicate [µg-at/l]" );
                s_Header.replace( "SIO3 [UG-AT/L]", "SiO3 [µg-at/l]" );
                s_Header.replace( "NH4 [UG-AT/L]", "Ammonium [µg-at/l]" );
                s_Header.replace( "POP [UG-AT/L]", "Phosphorus, organic, particulate [µg-at/l]" );
                s_Header.replace( "PON [UG-AT/L]", "Nitrogen, organic, particulate [µg-at/l]" );
                s_Header.replace( "TOTN [UG-AT/L]", "Nitrogen, total [µg-at/l]" );
                s_Header.replace( "TOTP [UG-AT/L]", "Phosphorus, total [µg-at/l]" );

                s_Header.replace( "PH []", "pH []" );
                s_Header.replace( "PO4 [UG/L]", "Phosphate [µg/l]" );
                s_Header.replace( "NO2 [UG/L]", "Nitrite [µg/l]" );
                s_Header.replace( "NO3 [UG/L]", "Nitrate [µg/l]" );
                s_Header.replace( "SIO4 [UG/L]", "Silicate [µg/l]" );

                s_Header.replace( "CHL A [MG/M3]", "Chlorophyll a [µg/l]" );
                s_Header.replace( "CHL B [MG/M3]", "Chlorophyll b [µg/l]" );
                s_Header.replace( "CHL C [MG/M3]", "Chlorophyll c [µg/l]" );
                s_Header.replace( "CHL A [UG/L]", "Chlorophyll a [µg/l]" );
                s_Header.replace( "CHL B [UG/L]", "Chlorophyll b [µg/l]" );
                s_Header.replace( "CHL C [UG/L]", "Chlorophyll c [µg/l]" );

                s_Header.replace( "PRIM PROD [MGC/M3/HR]", "Primary production of carbon per hour [mg/m**3/h]" );

                s_Header.replace( "ALK [MEQ/L]", "Alkalinity, total [mmol(eq)/l]" );
                s_Header.replace( "CORR []", "Correction []" );

                s_BaseDir = QString( "/hydrology_%1/" ).arg( i_Type );

                QDir().mkdir( QDir::toNativeSeparators( fi_out.absolutePath() + s_BaseDir ) );
                QDir().mkdir( QDir::toNativeSeparators( fi_out.absolutePath() + s_BaseDir + fi_in.baseName() ) );

                QFile fout( fi_out.absolutePath() + s_BaseDir + QString( "%1/%2_%1.txt" ).arg( fi_in.baseName() ).arg( ++k, 4, 10, QLatin1Char( '0' ) ) );

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

                tout << tr( "Event label\tStation\tDate/Time\tLatitude\tLongitude\tDepth, bathymetric [m]" ) << s_Header << s_EOL;

                i += 3;
                while ( ( i<n ) && ( sl_Input.at( i ).length() > 0 ) )
                {
                    s_Out = s_EventLabel + "\t" + s_Station + "\t" + s_DateTime + "\t" + llc->Latitude() + "\t" + llc->Longitude() + "\t" + s_BottomDepth + "\t" + sl_Input.at( i++ );

                    s_Out.replace( ",", "\t" );
                    tout << s_Out << s_EOL;
                }

                fout.close();
            }
            break;

        case _BENTHOS_:
            if ( sl_Input.at( i ).startsWith( "TYPE,BENTHOS" ) == true )
            {
                i++;

                s_BaseDir = tr( "/benthos/" );

                QDir().mkdir( QDir::toNativeSeparators( fi_out.absolutePath() + s_BaseDir ) );
                QDir().mkdir( QDir::toNativeSeparators( fi_out.absolutePath() + s_BaseDir + fi_in.baseName() ) );

                QFile fout( fi_out.absolutePath() + s_BaseDir + QString( "%1/%2_%1.txt" ).arg( fi_in.baseName() ).arg( ++k, 4, 10, QLatin1Char( '0' ) ) );

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

                m = sl_Input.at( i ).count( "," ) + 1;

                for ( int j=0; j<m; j++ )
                {
                    if ( sl_Input.at( i ).section( ",", j, j ) == "TAX NAME" )
                        m = j;
                }

                tout << tr( "Event label\tDate/Time\tLatitude|Longitude|Depth, bathymetric [m]|Station|Sample No.\tParameter\tValue" ) << s_EOL;

                s_Metadata = s_EventLabel + "\t" + s_DateTime + "\t" + llc->Latitude() + "|" + llc->Longitude() + "|" + s_BottomDepth + "|" + s_Station + "|";

                i += 3;

                while ( ( i<n ) && ( sl_Input.at( i ).length() > 0 ) )
                {
                    s_Out = "";

                    QString s_Species = sl_Input.at( i ).section( ",", m, m ).toLower();

                    s_Species.replace( " sp", " sp." );
                    s_Species.replace( " g.sp", "" );

                    if ( m > 0 )
                    {
                        s_Out.append( tr( "1\t" ) + s_Species + tr( " [#]\t" ) + sl_Input.at( i ).section( ",", 0, 0 ) + "|" );
                        s_Out.append( tr( "1\t" ) + s_Species + tr( ", wet mass [g]\t" ) + sl_Input.at( i ).section( ",", 1, 1 ) + "|"  );
                        s_Out.append( tr( "1\t" ) + s_Species + tr( ", biomass, wet mass [g/m**2]\t" ) + sl_Input.at( i ).section( ",", 2, 2 ) + "|" );
                    }

                    if ( m > 3 )
                    {
                        s_Out.append( tr( "2\t" ) + s_Species + tr( " [#]\t" ) + sl_Input.at( i ).section( ",", 3, 3 ) + "|" );
                        s_Out.append( tr( "2\t" ) + s_Species + tr( ", wet mass [g]\t" ) + sl_Input.at( i ).section( ",", 4, 4 ) + "|"  );
                        s_Out.append( tr( "2\t" ) + s_Species + tr( ", biomass, wet mass [g/m**2]\t" ) + sl_Input.at( i ).section( ",", 5, 5 ) + "|" );
                    }

                    if ( m > 6 )
                    {
                        s_Out.append( tr( "3\t" ) + s_Species + tr( " [#]\t" ) + sl_Input.at( i ).section( ",", 6, 6 ) + "|" );
                        s_Out.append( tr( "3\t" ) + s_Species + tr( ", wet mass [g]\t" ) + sl_Input.at( i ).section( ",", 7, 7 ) + "|"  );
                        s_Out.append( tr( "3\t" ) + s_Species + tr( ", biomass, wet mass [g/m**2]\t" ) + sl_Input.at( i ).section( ",", 8, 8 ) + "|" );
                    }

                    for ( int j=0; j<9; j++ )
                        tout << s_Metadata << s_Out.section( "|", j, j ) << s_EOL;

                    i++;
                }

                fout.close();
            }
            break;

        case _METEOROLOGY_:
            if ( sl_Input.at( i ).startsWith( "TYPE,METEO" ) == true )
            {
                s_BaseDir = tr( "/meteorology/" );

                QDir().mkdir( QDir::toNativeSeparators( fi_out.absolutePath() + s_BaseDir ) );
                QDir().mkdir( QDir::toNativeSeparators( fi_out.absolutePath() + s_BaseDir + fi_in.baseName() ) );

                QFile fout( fi_out.absolutePath() + s_BaseDir + QString( "%1/%2_%1.txt" ).arg( fi_in.baseName() ).arg( ++k, 4, 10, QLatin1Char( '0' ) ) );

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

                tout << tr( "Event label\tDate/Time\tLatitude|Longitude|Depth, bathymetric [m]|Station\tParameter\tValue" ) << s_EOL;

                s_Metadata = s_EventLabel + "\t" + s_DateTime + "\t" + llc->Latitude() + "|" + llc->Longitude() + "|" + s_BottomDepth + "|" + s_Station + "\t";

                i++;

                while ( ( i<n ) && ( sl_Input.at( i ).length() > 0 ) )
                {
                    s_Out = "";

                    s_Out.append( sl_Input.at( i ).section( ",", 0, 0 ) + " [" + sl_Input.at( i ).section( ",", 2, 2 ) + "]\t" + sl_Input.at( i ).section( ",", 1, 1 ) );

                    s_Out.replace( "WATER TRANS [M]", "Secchi depth, water transparency [m]" );
                    s_Out.replace( "AIRTEMP [C]", "Temperature, air [deg C]" );
                    s_Out.replace( "BARPRESS [MBAR]", "Pressure, atmospheric [hPa]" );
                    s_Out.replace( "WINDDIR [COMPASS]", "Wind direction description [] Compass" );
                    s_Out.replace( "WINDSP [M/SEC]", "Wind speed [m/s]" );
                    s_Out.replace( "VISIBILITY [INM]", "Visibility [in m]" );
                    s_Out.replace( "CLOUDAMT [CODE10]", "Cloud cover [octa]" );
                    s_Out.replace( "CLOUD TYPE [WMO0500]", "Cloud type [code] WMO0500" );
                    s_Out.replace( "CLOUD DOWN [CODE10]", "Cloud down [code]" );
                    s_Out.replace( "WATER COLOR []", "Color [#] of water" );
                    s_Out.replace( "SWELL [CODE]", "Swell [code]" );
                    s_Out.replace( "WAVEDIR [DEGREES]", "Wave direction [deg]" );
                    s_Out.replace( "WAVEHT [M]", "Wave height [m]" );
                    s_Out.replace( "WINDDIR [DEGREES]", "Wind direction [deg] Compass" );
                    s_Out.replace( "BARPRESS [MMHG]", "Pressure, atmospheric [mmHg]" );
                    s_Out.replace( "AIRTEMP DRY [C]", "Temperature, air [deg C] dry" );
                    s_Out.replace( "AIRTEMP WET [C]", "Temperature, air [deg C] wet" );
                    s_Out.replace( "SEA [CODE]", "Sea state [code]" );
                    s_Out.replace( "WAVEDIR [COMPASS]", "Wave direction description [deg] Compass" );
                    s_Out.replace( "WINDFOR [BEAUFORT]", "Wind speed [Bft]" );
                    s_Out.replace( "WAVEHT [WMO1555]", "Wave height [code] WMO1555" );
                    s_Out.replace( "REL HUMID [%]", "Humidity, relative [%]" );
                    s_Out.replace( "WEATHER []", "Present weather [code]" );
                    s_Out.replace( "VISIBILITY [KM]", "Visibility [km]" );
                    s_Out.replace( "WATER COLOR [FOREL-ULE]", "Color [#] of water on Forel-Ule scale" );
                    s_Out.replace( "VISIBILITY [CODE]", "Visibility [code] INM" );

                    tout << s_Metadata << s_Out << s_EOL;

                    i++;
                }

                fout.close();
            }
            break;

        case _PLANKTON_:
            if ( sl_Input.at( i ).startsWith( "TYPE,PLANKTON" ) == true )
            {
                i++;

                s_Header = "";

                m = sl_Input.at( i ).count( "," ) + 1;

                for ( int j=0; j<m; j++ )
                {
                    if ( sl_Input.at( i ).section( ",", j, j ).isEmpty() == false )
                        s_Header += "\t" + sl_Input.at( i ).section( ",", j, j );

                    if ( sl_Input.at( i ).section( ",", j, j ) == "TAX NAME" )
                        m = j;
                }

                s_Unit = " [" + sl_Input.at( i+1 ).section( ",", 0, 0 ) + "]";

                if ( ( sl_Input.at( i+1 ).section( ",", m, m ) == "PLANKTON" ) && ( s_Unit == s_Unit_requested ) )
                {
                    s_Header.replace( "TAX CNT B", "" );
                    s_Header.replace( "TAX PRESENT B", "" );
                    s_Unit.replace( "[*]", "[]" );

                    s_BaseDir = QString( "/plankton_%1/" ).arg( i_Type );

                    QDir().mkdir( QDir::toNativeSeparators( fi_out.absolutePath() + s_BaseDir ) );
                    QDir().mkdir( QDir::toNativeSeparators( fi_out.absolutePath() + s_BaseDir + fi_in.baseName() ) );

                    QFile fout( fi_out.absolutePath() + s_BaseDir + QString( "%1/%2_%1.txt" ).arg( fi_in.baseName() ).arg( ++k, 4, 10, QLatin1Char( '0' ) ) );

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

                    tout << tr( "Event label\tStation\tDate/Time\tLatitude\tLongitude\tDepth, bathymetric [m]\tDepth, water [m]" );

                    i += 3;
                    s = i;
                    s_Out = "";

                    while ( ( i<n ) && ( sl_Input.at( i ).length() > 0 ) )
                        s_Out.append( "\t" + sl_Input.at( i++ ).section( ",", m, m ) + s_Unit );

                    s_Out.replace( " SPP [", " spp. [");
                    s_Out.replace( " SP [", " sp. [");

                    tout << s_Out.toLower() << s_EOL;

                    s_Metadata = s_EventLabel + "\t" + s_Station + "\t" + s_DateTime + "\t" + llc->Latitude() + "\t" + llc->Longitude() + "\t" + s_BottomDepth + "\t";

                    for ( int j=m-1; j>=0; j-- )
                    {
                        s_Out = s_Metadata + s_Header.section( "\t", j+1, j+1 );

                        i = s;
                        while ( ( i<n ) && ( sl_Input.at( i ).length() > 0 ) )
                        {
                            if ( sl_Input.at( i ).section( ",", j, j ).isEmpty() == false )
                                s_Out.append( "\t" + sl_Input.at( i ).section( ",", j, j ) );
                            else
                                s_Out.append( "\t0" );

                            i++;
                        }

                        tout << s_Out << s_EOL;
                    }

                    fout.close();
                }
            }
            break;

        case _PHYTOPLANKTON1_:
        case _PHYTOPLANKTON2_:
            if ( sl_Input.at( i ).startsWith( "TYPE,PLANKTON" ) == true )
            {
                i++;

                s_Header = "";

                m = sl_Input.at( i ).count( "," ) + 1;

                for ( int j=0; j<m; j++ )
                {
                    if ( sl_Input.at( i ).section( ",", j, j ).isEmpty() == false )
                        s_Header += "\t" + sl_Input.at( i ).section( ",", j, j );

                    if ( sl_Input.at( i ).section( ",", j, j ) == "TAX NAME" )
                        m = j;
                }

                s_Unit = " [" + sl_Input.at( i+1 ).section( ",", 0, 0 ) + "]";

                if ( ( sl_Input.at( i+1 ).section( ",", m, m ) == "PHYTOPLANKTON" ) && ( s_Unit == s_Unit_requested ) )
                {
                    s_Header.replace( "TAX CNT B", "" );
                    s_Header.replace( "TAX PRESENT B", "" );
                    s_Unit.replace( "[*]", "[]" );

                    s_BaseDir = QString( "/phytoplankton_%1/" ).arg( i_Type );

                    QDir().mkdir( QDir::toNativeSeparators( fi_out.absolutePath() + s_BaseDir ) );
                    QDir().mkdir( QDir::toNativeSeparators( fi_out.absolutePath() + s_BaseDir + fi_in.baseName() ) );

                    QFile fout( fi_out.absolutePath() + s_BaseDir + QString( "%1/%2_%1.txt" ).arg( fi_in.baseName() ).arg( ++k, 4, 10, QLatin1Char( '0' ) ) );

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

                    tout << tr( "Event label\tStation\tDate/Time\tLatitude\tLongitude\tDepth, bathymetric [m]\tDepth, water [m]" );

                    i += 3;
                    s = i;
                    s_Out = "";

                    while ( ( i<n ) && ( sl_Input.at( i ).length() > 0 ) )
                        s_Out.append( "\t" + sl_Input.at( i++ ).section( ",", m, m ) + s_Unit );

                    s_Out.replace( " SPP [", " spp. [");
                    s_Out.replace( " SP [", " sp. [");

                    tout << s_Out.toLower() << s_EOL;

                    s_Metadata = s_EventLabel + "\t" + s_Station + "\t" + s_DateTime + "\t" + llc->Latitude() + "\t" + llc->Longitude() + "\t" + s_BottomDepth + "\t";

                    for ( int j=m-1; j>=0; j-- )
                    {
                        s_Out = s_Metadata + s_Header.section( "\t", j+1, j+1 );

                        i = s;
                        while ( ( i<n ) && ( sl_Input.at( i ).length() > 0 ) )
                        {
                            if ( sl_Input.at( i ).section( ",", j, j ).isEmpty() == false )
                                s_Out.append( "\t" + sl_Input.at( i ).section( ",", j, j ) );
                            else
                                s_Out.append( "\t0" );

                            i++;
                        }

                        tout << s_Out << s_EOL;
                    }

                    fout.close();
                }
            }
            break;

        case _ZOOPLANKTON1_:
        case _ZOOPLANKTON2_:
        case _ZOOPLANKTON3_:
        case _ZOOPLANKTON4_:
            if ( sl_Input.at( i ).startsWith( "TYPE,PLANKTON" ) == true )
            {
                i++;

                s_Header = "";

                m = sl_Input.at( i ).count( "," ) + 1;

                for ( int j=0; j<m; j++ )
                {
                    if ( sl_Input.at( i ).section( ",", j, j ).isEmpty() == false )
                        s_Header += "\t" + sl_Input.at( i ).section( ",", j, j );

                    if ( sl_Input.at( i ).section( ",", j, j ) == "TAX NAME" )
                        m = j;
                }

                s_Unit = " [" + sl_Input.at( i+1 ).section( ",", 0, 0 ) + "]";

                if ( ( sl_Input.at( i+1 ).section( ",", m, m ) == "ZOOPLANKTON" ) && ( s_Unit == s_Unit_requested ) )
                {
                    if ( s_Header.contains( "TOTAL WET B" ) == true )
                        s_Comment = "@total wet";

                    s_Header.replace( "TOTAL WET B", "" );
                    s_Header.replace( "TAX CNT B", "" );
                    s_Header.replace( "TAX PRESENT B", "" );
                    s_Unit.replace( "[*]", "[]" );
                    s_Unit.replace( "/SAMPLE", "" );
                    s_Unit.replace( "/M3", "/m**3" );

                    s_BaseDir = QString( "/zooplankton_%1/" ).arg( i_Type );

                    QDir().mkdir( QDir::toNativeSeparators( fi_out.absolutePath() + s_BaseDir ) );
                    QDir().mkdir( QDir::toNativeSeparators( fi_out.absolutePath() + s_BaseDir + fi_in.baseName() ) );

                    QFile fout( fi_out.absolutePath() + s_BaseDir + QString( "%1/%2_%1.txt" ).arg( fi_in.baseName() ).arg( ++k, 4, 10, QLatin1Char( '0' ) ) );

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

                    tout << tr( "Event label\tStation\tDate/Time\tLatitude\tLongitude\tDepth, bathymetric [m]\tDepth, water [m]" );

                    i += 3;
                    s = i;
                    s_Out = "";

                    while ( ( i<n ) && ( sl_Input.at( i ).length() > 0 ) )
                        s_Out.append( "\t" + sl_Input.at( i++ ).section( ",", m, m ) + s_Unit + s_Comment );

                    s_Out.replace( " SPP [", " spp. [");
                    s_Out.replace( " SP [", " sp. [");

                    tout << s_Out.toLower() << s_EOL;

                    s_Metadata = s_EventLabel + "\t" + s_Station + "\t" + s_DateTime + "\t" + llc->Latitude() + "\t" + llc->Longitude() + "\t" + s_BottomDepth + "\t";

                    for ( int j=m-1; j>=0; j-- )
                    {
                        s_Out = s_Metadata + s_Header.section( "\t", j+1, j+1 );

                        i = s;
                        while ( ( i<n ) && ( sl_Input.at( i ).length() > 0 ) )
                        {
                            if ( sl_Input.at( i ).section( ",", j, j ).isEmpty() == false )
                                s_Out.append( "\t" + sl_Input.at( i ).section( ",", j, j ) );
                            else
                                s_Out.append( "\t0" );

                            i++;
                        }

                        tout << s_Out << s_EOL;
                    }

                    fout.close();
                }
            }
            break;

        default:
            ;
            break;
        }

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doConvertNOAA_IOAS( const int i_Type )
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_EOL, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Convert NOAA-IOAS data..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_EOL, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = convertNOAA_IOAS( i_Type, s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gsl_FilenameList.count() );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_EOL, gsl_FilenameList, tr( "Done" ), tr( "Converting NOAA-IOAS data was canceled" ) );

    onError( err );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doConvertNOAA_IOAS_Benthos()
{
    doConvertNOAA_IOAS( _BENTHOS_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doConvertNOAA_IOAS_Hydrology()
{
    doConvertNOAA_IOAS( _HYDROLOGY_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doConvertNOAA_IOAS_Meteorology()
{
    doConvertNOAA_IOAS( _METEOROLOGY_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doConvertNOAA_IOAS_Plankton() // Plankton []
{
    doConvertNOAA_IOAS( _PLANKTON_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doConvertNOAA_IOAS_Phytoplankton1() // Phytoplankton [#/l]
{
    doConvertNOAA_IOAS( _PHYTOPLANKTON1_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doConvertNOAA_IOAS_Phytoplankton2() // Phytoplankton []
{
    doConvertNOAA_IOAS( _PHYTOPLANKTON2_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doConvertNOAA_IOAS_Zooplankton1() // Zooplankton [#]
{
    doConvertNOAA_IOAS( _ZOOPLANKTON1_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doConvertNOAA_IOAS_Zooplankton2() // Zooplankton [#/m**3]
{
    doConvertNOAA_IOAS( _ZOOPLANKTON2_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doConvertNOAA_IOAS_Zooplankton3() // Zooplankton [mg]
{
    doConvertNOAA_IOAS( _ZOOPLANKTON3_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

void MainWindow::doConvertNOAA_IOAS_Zooplankton4() // Zooplankton []
{
    doConvertNOAA_IOAS( _ZOOPLANKTON4_ );
}
