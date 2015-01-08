/* 2008-04-07                 */
/* Dr. Rainer Sieger          */

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

int MainWindow::CliwocConverter( const QString& s_FilenameIn, const QString& s_FilenameOut,  const QString& s_FilenameConf, const int i_NumOfFiles )
{
    int             k               = 0;
    int             n               = 0;

    QString         InputStr        = "";
    QString         s_EventLabel    = "";

    QStringList     sl_Conf;

    unsigned int	ui_length		= 1;
    unsigned int	ui_filesize		= 1;

// **********************************************************************************************

    n = readFile( s_FilenameConf, sl_Conf );

    if ( n < 9 )
        return( _ERROR_ );

// **********************************************************************************************
// open input file

    QFile fin( s_FilenameIn );

    if ( fin.open( QIODevice::ReadOnly | QIODevice::Text ) == false )
        return( -10 );

    ui_filesize = fin.size();

    QTextStream tin( &fin );

// **********************************************************************************************
// open output file

    QFile fout( s_FilenameOut );

    if ( fout.open( QIODevice::WriteOnly | QIODevice::Text ) == false )
        return( -20 );

    QTextStream tout( &fout );

// **********************************************************************************************

    initProgress( i_NumOfFiles, s_FilenameIn, tr( "Converting..." ), 100 );

// **********************************************************************************************
// read file

// Header
    tout << k++ << "\t" << "Event label" << "\t" << "Date/Time" << "\t" << "Latitude" << "\t" << "Longitude";

    for ( int i=9; i<n; i++ )
    {
        if ( sl_Conf.at( i ).section( "\t", 0, 0 ) == "x" )
            tout << "\t" << sl_Conf.at( i ).section( "\t", 3, 3 );
    }

    tout << endl;

    while ( ( tin.atEnd() == false ) && ( ui_length != (unsigned int) _APPBREAK_ ) )
    {
        InputStr  = tin.readLine();
        ui_length = incProgress( i_NumOfFiles, ui_filesize, ui_length, InputStr );

// Event label
        s_EventLabel = "@" + InputStr.mid( 34, 9 ).simplified() + "_" + InputStr.mid( 2597, 8 ).simplified() + "_" + InputStr.mid( 297, 30 ).simplified();

        s_EventLabel.replace( "EXTRACT ", "" );
        s_EventLabel.replace( ",", "" );
        s_EventLabel.replace( ".", "" );
        s_EventLabel.replace( "(", "-" );
        s_EventLabel.replace( ")", "" );
        s_EventLabel.replace( " ", "_" );
        s_EventLabel.replace( "_-_", "_" );
        s_EventLabel.replace( "_-", "_" );
        s_EventLabel.replace( "-_", "_" );
        s_EventLabel.replace( "__", "_" );
        s_EventLabel.replace( "'", "" );

        tout << k++ << "\t" << s_EventLabel << "\t";

// Date/Time
        QString s_DateTime = InputStr.mid( 0, 4 ) + tr( "-" ) + InputStr.mid( 4, 2 ) + tr( "-" ) + InputStr.mid( 6, 2 ) + tr( "T" ) + InputStr.mid( 8, 2 ) + tr( ":" ) + InputStr.mid( 10, 2 );
        s_DateTime.replace( " ", "0" );
        tout << s_DateTime << "\t";

// Position
        // Latitude
        if ( InputStr.mid( 12, 5 ) != "     " )
            tout << InputStr.mid( 12, 5 ).toFloat()/100. << "\t";
        else
            tout << "\t";

        // Longitude
        if ( InputStr.mid( 17, 6 ) != "      " )
        {
            float f_Longitude = InputStr.mid( 17, 6 ).toFloat()/100.;

            if ( f_Longitude > 180. )
                f_Longitude -=360;

            tout << f_Longitude;
        }
        else
            tout << "";

        for ( int i=9; i<n; i++ )
        {
            if ( sl_Conf.at( i ).section( "\t", 0, 0 ) == "x" )
            {
                QString s_Factor = sl_Conf.at( i ).section( "\t", 1, 1 );
                QString s_Entry  = InputStr.mid( sl_Conf.at( i ).section( "\t", 4, 4 ).toInt()-1, sl_Conf.at( i ).section( "\t", 6, 6 ).toInt() ).simplified();

                if ( s_Entry.isEmpty() == true )
                {
                    tout << "\t";
                }
                else
                {
                    if ( s_Factor == "T" )
                    {
                        s_Entry.replace( "\"", "''" );
                        tout << "\t~@" << s_Entry;
                    }
                    else
                    {
                        if ( s_Factor.isEmpty() == false )
                            tout << "\t" << s_Entry.toFloat()*s_Factor.toFloat();
                        else
                            tout << "\t" << s_Entry;
                    }
                }
            }
        }

        tout << endl;
    }

    resetProgress( i_NumOfFiles );

    fin.close();
    fout.close();

// **********************************************************************************************

    if ( ui_length == (unsigned int) _APPBREAK_ )
        return( _APPBREAK_ );

    return( _NOERROR_ );
}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2008-04-07

void MainWindow::doCliwocConverter()
{
    int		i				= 0;
    int		err				= 0;
    int		stopProgress	= 0;

    QString s_FilenameIn    = "";
    QString s_FilenameOut   = "";
    QString s_FilenameConf  = "";

// **********************************************************************************************

    if ( existsFirstFile( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList ) == true )
    {
        QFileInfo fi( gsl_FilenameList.at( 0 ) );

        s_FilenameConf = QFileDialog::getOpenFileName( this, tr( "Choose parameter config file" ), fi.absolutePath() + tr( "/Parameter.conf" ), tr( "Config (*.conf)" ) );

        if ( s_FilenameConf.isEmpty() == false )
        {
            initFileProgress( gsl_FilenameList.count(), gsl_FilenameList.at( 0 ), tr( "CLIWOC converter working..." ) );

            while ( ( i < gsl_FilenameList.count() ) && ( err == _NOERROR_ ) && ( stopProgress != _APPBREAK_ ) )
            {
                if ( buildFilename( gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList.at( i ), s_FilenameIn, s_FilenameOut ) == true )
                {
                    err = CliwocConverter( s_FilenameIn, s_FilenameOut, s_FilenameConf, gsl_FilenameList.count() );

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
    }
    else
    {
        err = _CHOOSEABORTED_;
    }

// **********************************************************************************************

    endTool( err, stopProgress, gi_ActionNumber, gs_FilenameFormat, gi_Extension, gsl_FilenameList, tr( "Done" ), tr( "CLIWOC converter was canceled" ) );

    onError( err );
}

