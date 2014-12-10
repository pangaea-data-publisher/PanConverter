/* SOCAT 1.4 converter        */
/* 2008-07-23                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::convertSOCAT( const QString &s_FilenameIn, const QString &s_FilenameOut, const int i_CodecInput, const int i_CodecOutput, const int i_EOL, const int i_NumOfFiles )
{
    int         i               = 0;
    int         n               = 0;

    int         stopProgress    = 0;

    float       f_bathyDepth    = 0;
    float       f_Depth         = 0;

    QString     s_EOL           = setEOLChar( i_EOL );

    QStringList sl_Input;

// **********************************************************************************************
// read file

    if ( ( n = readFile( s_FilenameIn, sl_Input, i_CodecInput ) ) < 1 )
        return( n );

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

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting SOCAT data..." ), sl_Input.count() );

    tout << "Event label\t";                    // 0
//  tout << "cruise_name\t";                    // 1
//  tout << "Area\t";                           // 2
//  tout << "Label\t";                          // 3
//  tout << "type\t";                           // 4

//  tout << "mon\t";                            // 5
//  tout << "day\t";                            // 6
//  tout << "yr\t";                             // 7
//  tout << "hh\t";                             // 8
//  tout << "mm\t";                             // 9

    tout << "Date/Time\t";                      // 7, 5, 6, 8, 9

    tout << "Latitude\t";                       // 11
    tout << "Longitude\t";                      // 10

    tout << "Depth, bathymetric [m]\t";         // 12
    tout << "Depth, water [m]\t";               // 13
//  tout << "depthW\t";                         // 14

    tout << "Temperature, water [deg C]\t";     // 15
    tout << "Salinity []\t";                    // 16

    tout << "xCO2 (water) at sea surface temperature (wet air) [mol/mol]\t";                                                       // 17
    tout << "xCO2 (water) at equilibrator temperature (wet air) [mol/mol]\t";                                                      // 18
    tout << "xCO2 (water) at sea surface temperature (dry air) [mol/mol]\t";                                                       // 19
    tout << "xCO2 (water) at equilibrator temperature (dry air) [mol/mol]\t";                                                      // 20

    tout << "Fugacity of CO2 (water) at sea surface temperature (wet air) [atm]\t";                                                // 21
//  tout << "fCO2_water_corr_25_uatm\t";                                                                                           // 22
//  tout << "fCO2_water_corr_to_20\t";                                                                                             // 23
    tout << "Fugacity of CO2 (water) at equilibrator temperature (wet air) [atm]\t";                                               // 24

    tout << "Partial pressure of CO2 (water) at sea surface temperature (wet air) [atm]\t";                                        // 25
    tout << "Partial pressure of CO2 (water) at equilibrator temperature (wet air) [atm]\t";                                       // 26
    tout << "Partial pressure of CO2 (water) at sea surface temperature (wet air) [atm]@corrected to sea surface temperature\t";   // 27

    tout << "Temperature at equilibration [C]\t";                                                                                  // 28
    tout << "Pressure, atmospheric [hPa]\t";                                                                                       // 29
    tout << "Pressure at equilibration [hPa]\t";                                                                                   // 30

//  tout << "Wind direction [deg]\t";                                                                                              // 31
//  tout << "Wind speed [m/s]\t";                                                                                                  // 32
//  tout << "Speed [kn]@of ship\t";                                                                                                // 33
//  tout << "Direction [deg]@of ship\t";                                                                                           // 34
//  tout << "Humidity, relative [%]\t";                                                                                            // 35

    tout << "Salinity, interpolated []@WOA\t";                                                                                     // 36
//  tout << "woa_land_marker\t";                                                                                                   // 37
    tout << "Pressure, atmospheric, interpolated [hPa]@NCEP\t";                                                                    // 38
//  tout << "Speed [kn]@calculated\t";                                                                                             // 39
    tout << "Depth, bathymetric, interpolated [m]@ETOPO\t";                                                                        // 40

//  tout << "fCO2_insitu_from_xCO2_water_equi_temp_dry_ppm\t";                                                                     // 41
//  tout << "fCO2_insitu_from_xCO2_water_sst_dry_ppm\t";                                                                           // 42
//  tout << "fCO2_from_pCO2_water_water_equi_temp\t";                                                                              // 43
//  tout << "fCO2_from_pCO2_water_sst_100humidity_uatm\t";                                                                         // 44
//  tout << "fCO2_insitu_from_fCO2_water_equi_uatm\t";                                                                             // 45
//  tout << "fCO2_insitu_from_fCO2_water_sst_100humidty_uatm\t";                                                                   // 46
//  tout << "fCO2_from_pCO2_water_water_equi_temp_ncep\t";                                                                         // 47
//  tout << "fCO2_from_pCO2_water_sst_100humidity_uatm_ncep\t";                                                                    // 48
//  tout << "fCO2_insitu_from_xCO2_water_equi_temp_dry_ppm_woa\t";                                                                 // 49
//  tout << "fCO2_insitu_from_xCO2_water_sst_dry_ppm_woa\t";                                                                       // 50
//  tout << "fCO2_insitu_from_xCO2_water_equi_temp_dry_ppm_ncep\t";                                                                // 51
//  tout << "fCO2_insitu_from_xCO2_water_sst_dry_ppm_ncep\t";                                                                      // 52
//  tout << "fCO2_insitu_from_xCO2_water_equi_temp_dry_ppm_ncep_woa\t";                                                            // 53
//  tout << "fCO2_insitu_from_xCO2_water_sst_dry_ppm_ncep_woa\t";                                                                  // 54

    tout << "Fugacity of CO2 (water) at sea surface temperature (wet air) [atm]@recommended \t";                                   // 55

//  tout << "fCO2_source\t";                                                                                                       // 56
//  tout << "gvco2\t";                                                                                                             // 57
//  tout << "d2l\t";                                                                                                               // 58
//  tout << "Julian_day_GMT\t";                                                                                                    // 59
//  tout << "deltaT\t";                                                                                                            // 60
    tout << "Quality flag [#]@3: questionable measurements; 4: bad measurements";                                                  // 61
    tout  << s_EOL;

    i = 4;

    while ( ( i < n ) && ( stopProgress != _APPBREAK_ ) )
    {
        QString s_Output = "";

        f_bathyDepth = -999;
        f_Depth      = -999;

        if ( sl_Input.at( i ).section( "\t", 14, 14 ) == "NaN" )
            f_Depth = sl_Input.at( i ).section( "\t", 13, 13 ).toFloat();
        else
            f_Depth = sl_Input.at( i ).section( "\t", 14, 14 ).toFloat();

        if ( sl_Input.at( i ).section( "\t", 12, 12 ).isEmpty() == false )
            f_bathyDepth = sl_Input.at( i ).section( "\t", 12, 12 ).toFloat();

        if ( f_bathyDepth <= f_Depth )
            f_bathyDepth = -999;

        s_Output.append( sl_Input.at( i ).section( "\t", 0, 0 ) + "-track\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 1, 1 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 2, 2 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 3, 3 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 4, 4 ) + "\t" );

        QDate date = QDate( sl_Input.at( i ).section( "\t", 7, 7 ).toInt(), sl_Input.at( i ).section( "\t", 5, 5 ).toInt(), sl_Input.at( i ).section( "\t", 6, 6 ).toInt() );
        QTime time = QTime( sl_Input.at( i ).section( "\t", 8, 8 ).toInt(), sl_Input.at( i ).section( "\t", 9, 9 ).toInt(), 0, 0 );

        if ( ( date.isValid() == true ) && ( time.isValid() == true ) )
            s_Output.append( date.toString( Qt::ISODate ) + "T" + time.toString( Qt::ISODate ) + "\t" );
        else
            s_Output.append( tr( "wrong date and/or time\t" ) );

        if ( sl_Input.at( i ).section( "\t", 11, 11 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 11, 11 ).toFloat(), 0, 'f', 4 ) + "\t" );
        else
            s_Output.append( "\t" );

        if ( sl_Input.at( i ).section( "\t", 10, 10 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 10, 10 ).toFloat(), 0, 'f', 4 ) + "\t" );
        else
            s_Output.append( "\t" );

        if ( f_bathyDepth > 0 )
            s_Output.append( QString( "%1" ).arg( f_bathyDepth, 0, 'f', 0 )  + "\t" );
        else
            s_Output.append( "\t" );

        s_Output.append( QString( "%1" ).arg( f_Depth, 0, 'f', 0 )  + "\t" );

        if ( sl_Input.at( i ).section( "\t", 15, 15 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 15, 15 ).toFloat(), 0, 'f', 3 ) + "\t" );
        else
            s_Output.append( "\t" );

        if ( sl_Input.at( i ).section( "\t", 16, 16 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 16, 16 ).toFloat(), 0, 'f', 3 ) + "\t" );
        else
            s_Output.append( "\t" );

        if ( sl_Input.at( i ).section( "\t", 17, 17 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 17, 17 ).toFloat(), 0, 'f', 2 ) + "\t" );
        else
            s_Output.append( "\t" );

        if ( sl_Input.at( i ).section( "\t", 18, 18 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 18, 18 ).toFloat(), 0, 'f', 2 ) + "\t" );
        else
            s_Output.append( "\t" );

        if ( sl_Input.at( i ).section( "\t", 19, 19 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 19, 19 ).toFloat(), 0, 'f', 2 ) + "\t" );
        else
            s_Output.append( "\t" );

        if ( sl_Input.at( i ).section( "\t", 20, 20 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 20, 20 ).toFloat(), 0, 'f', 2 ) + "\t" );
        else
            s_Output.append( "\t" );

        if ( sl_Input.at( i ).section( "\t", 21, 21 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 21, 21 ).toFloat(), 0, 'f', 2 ) + "\t" );
        else
            s_Output.append( "\t" );

//      s_Output.append( sl_Input.at( i ).section( "\t", 22, 22 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 23, 23 ) + "\t" );

        if ( sl_Input.at( i ).section( "\t", 24, 24 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 24, 24 ).toFloat(), 0, 'f', 2 ) + "\t" );
        else
            s_Output.append( "\t" );

        if ( sl_Input.at( i ).section( "\t", 25, 25 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 25, 25 ).toFloat(), 0, 'f', 2 ) + "\t" );
        else
            s_Output.append( "\t" );

        if ( sl_Input.at( i ).section( "\t", 26, 26 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 26, 26 ).toFloat(), 0, 'f', 2 ) + "\t" );
        else
            s_Output.append( "\t" );

        if ( sl_Input.at( i ).section( "\t", 27, 27 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 27, 27 ).toFloat(), 0, 'f', 2 ) + "\t" );
        else
            s_Output.append( "\t" );

        if ( sl_Input.at( i ).section( "\t", 28, 28 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 28, 28 ).toFloat(), 0, 'f', 3 ) + "\t" );
        else
            s_Output.append( "\t" );

        if ( sl_Input.at( i ).section( "\t", 29, 29 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 29, 29 ).toFloat(), 0, 'f', 3 ) + "\t" );
        else
            s_Output.append( "\t" );

        if ( sl_Input.at( i ).section( "\t", 30, 30 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 30, 30 ).toFloat(), 0, 'f', 3 ) + "\t" );
        else
            s_Output.append( "\t" );

//      s_Output.append( sl_Input.at( i ).section( "\t", 31, 31 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 32, 32 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 33, 33 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 34, 34 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 35, 35 ) + "\t" );

        if ( sl_Input.at( i ).section( "\t", 36, 36 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 36, 36 ).toFloat(), 0, 'f', 2 ) + "\t" );
        else
            s_Output.append( "\t" );

//      s_Output.append( sl_Input.at( i ).section( "\t", 37, 37 ) + "\t" );

        if ( sl_Input.at( i ).section( "\t", 38, 38 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 38, 38 ).toFloat(), 0, 'f', 2 ) + "\t" );
        else
            s_Output.append( "\t" );

//      s_Output.append( sl_Input.at( i ).section( "\t", 39, 39 ) + "\t" );

        if ( sl_Input.at( i ).section( "\t", 40, 40 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( -1 * sl_Input.at( i ).section( "\t", 40, 40 ).toFloat(), 0, 'f', 0 ) + "\t" );
        else
            s_Output.append( "\t" );

//      s_Output.append( sl_Input.at( i ).section( "\t", 41, 41 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 42, 42 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 43, 43 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 44, 44 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 45, 45 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 46, 46 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 47, 47 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 48, 48 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 49, 49 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 50, 50 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 51, 51 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 52, 52 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 53, 53 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 54, 54 ) + "\t" );

        if ( sl_Input.at( i ).section( "\t", 55, 55 ).isEmpty() == false )
            s_Output.append( QString( "%1" ).arg( sl_Input.at( i ).section( "\t", 55, 55 ).toFloat(), 0, 'f', 2 ) + "\t" );
        else
            s_Output.append( "\t" );

//      s_Output.append( sl_Input.at( i ).section( "\t", 56, 56 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 57, 57 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 58, 58 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 59, 59 ) + "\t" );
//      s_Output.append( sl_Input.at( i ).section( "\t", 60, 60 ) + "\t" );

        if ( sl_Input.at( i ).section( "\t", 61, 61 ) != "2" )
            s_Output.append( sl_Input.at( i ).section( "\t", 61, 61 ) );

        s_Output.replace( "NaN", "" );
        s_Output.replace( "nan", "" );

        tout << s_Output << s_EOL;

        stopProgress = incProgress( i_NumOfFiles, ++i );
    }

    resetProgress( i_NumOfFiles );

// **********************************************************************************************

    fout.close();

    if ( stopProgress == _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doConvertSOCAT()
{
    int     i               = 0;
    int     err             = 0;
    int     stopProgress    = 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "Converting SOCAT data..." ) );

        while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
        {
            if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
            {
                err = convertSOCAT( s_FilenameIn, s_FilenameOut, gi_CodecInput, gi_CodecOutput, gi_EOL, gsl_FilenameList.count() );

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

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "Converting SOCAT data was canceled" ) );

    onError( err );
}

