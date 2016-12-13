/* Application.h              */
/* 2013-05-15                 */
/* Dr. Rainer Sieger          */

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtXml/QtXml>

#include <QTextEdit>
#include <QTextCodec>
#include <QTextStream>
#include <QStringListModel>
#include <QListView>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QCloseEvent>
#include <QProcess>
#include <QTimer>
#include <QTime>
#include <QMimeData>
#include <QDesktopServices>
#include <QMessageBox>
#include <QProgressBar>
#include <QProgressDialog>
#include <QStatusBar>
#include <QSettings>
#include <QSslError>

#include <qmath.h>

#include "Globals.h"

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class QAction;
class QLabel;
class QProgressDialog;
class QProgressBar;
class QTimer;
class QDateTime;
class QUrl;
class QTextEdit;
class QStringListModel;
class QListView;
class QTableWidget;
class QXml;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_CLASSINFO( "author", "Rainer Sieger" )
    Q_CLASSINFO( "url", "https://pangaea.de" )

public:
    explicit MainWindow( QWidget *parent = 0 );
    ~MainWindow();

    QStringList gsl_FilenameList;                   //!< Liste aller Dateinamen incl. Pfad.

    // General settings
    QString     gs_Path;                            //!< Der zuletzt verwendete Pfad.
    QString     gs_FilenameFormat;                  //!< Bestimmt in die Form des automatisch erzeugten neuen Dateinamens. \%a = aktuelle Actionnumber, \%N = Dateiname.
    QString     gs_FilenameGoogleEarthProgram;      //!< Pfad und Name des Google Earth Programmes.
    QString     gs_Version;                         //!< Aktuelle Version des Programms.

    bool        gb_showProgressBar;                 //!< Soll der ProgressBar im Fenster unten rechts angezeigt werden?

    int         gi_ActionNumber;                    //!< Die ActionNumber wird bei jedem angewandenten Tool um 1 erhoeht.
    int         gi_NumOfProgramStarts;              //!< Anzahl der Programmstarts
    int         gi_CodecInput;                      //!< Encoding der Inputdateien
    int         gi_CodecOutput;                     //!< Encoding der Outputdateien
    int         gi_CodecDownload;                   //!< Encoding der Downloaddateien
    int         gi_EOL;                             //!< End-of-Line Zeichen fuer die Outputdateien
    int         gi_Extension;                       //!< Dateierweiterung fuer Ausgabedateien

    int         gi_IconSize;                        //!< Groesse der Marker bei KML
    int         gi_IconColor;                       //!< Farbe der Marker bei KML
    int         gi_TracklineWidth;                  //!< Breite der Trackline bei KML
    int         gi_TracklineColor;                  //!< Farbe der Trackline bei KML

    bool        gb_startGoogleEarth;                //!< true wenn Google Earth automatisch gestart werden soll.

    bool LineCanBeWritten( const QString &InputStr, const bool SkipEmptyLines, const bool SkipCommentLines );
    bool isInColumnList( const QList<int> ColumnList, const int ColumnNo );
    QList<int> scanList( const int mode, const int maxNumOfPositions, const QString &List );

    double calculateDistance( const double Latitude1, const double Longitude1, const double Latitude2, const double Longitude2 );

    QDateTime setDateTime( const QString DateTime );

    int concatenateFilesByLines( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QStringList &FilenameList, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int SkipNStartLines, const bool IncludeFilename, const bool SkipEmptyLines, const bool SkipCommentLines, const bool b_DeleteInputFile );
    int concatenateFilesByColumns( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QStringList &FilenameList, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int SkipNStartLines, const bool IncludeFilename, const bool DeleteInputFile );

    int parseCrossRefXML( const QString &FilenameIn, const int CodecOutput, const int EOL, const int Extension, const int NumOfFiles );
    int parseTreeRingXML( const QString &FilenameIn, const int CodecOutput, const int EOL, const int Extension, QStringList &Filenames_events, QStringList &Filenames_metadata, QStringList &Filenames_data, const int NumOfFiles );
    int parseTextEntry(const QDomElement &element, QString &Content );
    int parseTextEntry( const QDomElement &element, const QString &Attribute, QString &AttributeContent, QString &Content );
    int parseNumericEntry( const QDomElement &element, int &Content );
    int parseNumericEntry( const QDomElement &element, const QString &Attribute, QString &AttributeContent, int &Content );
    int parseValueEntry( const QDomElement &element );
    int convertQualityFlag( QString &QualityFlag );

    int convertCHUAN_station( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int NumOfFiles );
    int convertCHUAN_moving( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int NumOfFiles );
    int convertSOCAT( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int NumOfFiles );
    int convertMastertrack( const QString &FilenameIn, const int CodecInput, const int CodecOutput, const int EOL, const QStringList &crInput, const int NumOfFiles );
    int createMastertrackImportFile(const QString &FilenameIn, const int CodecInput, const int CodecOutput, const QStringList &crInput, const int Resolution, const int NumOfFiles );
    int createMastertrackImportFile( const int Resolution );
    int convertNOAA_IOAS( const int Type, const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int NumOfFiles );
    int convertTSG( const QString &FilenameIn, const QString &FilenameOut, const int CodecInput, const int CodecOutput, const int EOL, const int NumOfFiles );
    int CliwocConverter( const QString &FilenameIn, const QString &FilenameOut, const QString &FilenameConf, const int NumOfFiles );
    int WOCEWHPConverter( const QString &FilenameIn, const QString &FilenameOut );

    int doGPStoGpxOptionsDialog( int &StartID, int &EndID, QString &TrackName );
    int doGPStoKmlOptionsDialog( int &StartID, int &EndID, int &IconColor, int &IconSize, int &TracklineColor, int &TracklineWidth, QString &TrackName, QString &FilenameGoogleEarthProgram, bool &startGoogleEarth );
    int createGpxFile( const QString &FilenameIn, const int NumOfFiles );
    int createKmlFile( const QString &FilenameIn, int &IconColor, int &IconSize, int &TracklineColor, int &TracklineWidth, QString &FilenameGoogleEarthProgram, bool &startGoogleEarth, QString &FilenameOut, const int NumOfFiles );

    int startProgram( const QString &Program, const QString &Filename );
    int startGoogleEarth( const QString &Program, const QString &Filename );

    QString extractLatitude( const QString &InputStr );
    QString extractLongitude( const QString &InputStr );

    QString findBasis( const QString &Filename );
    QString findExpedition( const QString &Filename, const QStringList &sl_crInput );

    int findExpeditionID( const QString &Expedition, const QStringList &crInput );

    QString OpenDataDescriptionHeader();
    QString CloseDataDescriptionHeader();
    QString ParentID( const QString &ParentID = "-999" );
    QString DataSetID( const QString &DatasetID = "-999" );
    QString AuthorIDs( const QString &AuthorIDs = "-999" );
    QString SourceID( const QString &SourceID = "-999" );
    QString DatasetTitle( const QString &Text );
    QString Reference( const QString &ReferenceID = "-999", const int RelationTypeID = -999, const QString &ReferenceType = "-999", const QString &EventLabel = "" );
    QString ExportFilename( const QString &EventLabel, const QString &Text );
    QString EventLabel( const QString &EventLabel = "" );
    QString Parameter( const QString &ParameterID = "-999", const QString &PIID = "506", const QString &MethodID = "43", const QString &Format = "", const QString &Comment = "" );
    QString Parameter( const QStringList &Parameter );
    QString DatasetComment( const QString &DatasetComment = "" );
    QString ProjectIDs( const QString &ProjectID = "-999" );
    QString TopologicTypeID( const QString &TopologicTypeID = "-999" );
    QString StatusID( const QString &StatusID = "-999" );
    QString UserIDs( const QString &UserIDs = "-999" );
    QString LoginID( const QString &LoginID = "-999" );

    QString num2str( const int num ) { return( QString::number( num ) ); }
    QString num2str( const float num ) { return( QString::number( num ) ); }
    QString num2str( const double num ) { return( QString::number( num ) ); }

protected:
    void dragEnterEvent( QDragEnterEvent *event );
    void dropEvent( QDropEvent *event );
    void closeEvent( QCloseEvent *event );
    void onError( const int err );

signals:
    void finishedBuildFilelist( bool );

private slots:
    int  chooseFile();
    int  chooseFiles();
    int  chooseFolder( const QString &Folder = "" );
    int  saveFile();
    int  saveFileAs();

    void exitApplication();
    void hideWindow();
    void newWindow();
    void displayHelp();
    void about();
    void clearStatusMessage();

    void doCompressFiles();

    void doParseTreeRingXML();
    void doParseCrossRefXML();
    void doConvertCHUAN_station();
    void doConvertCHUAN_moving();
    void doConvertMastertrack();
    void doCreateMastertrackImportFile_30sec();
    void doCreateMastertrackImportFile_1min();
    void doCreateMastertrackImportFile_10min();

    void doConvertNOAA_IOAS( const int Type );
    void doConvertNOAA_IOAS_Benthos();
    void doConvertNOAA_IOAS_Hydrology();
    void doConvertNOAA_IOAS_Meteorology();
    void doConvertNOAA_IOAS_Plankton();
    void doConvertNOAA_IOAS_Phytoplankton1();
    void doConvertNOAA_IOAS_Phytoplankton2();
    void doConvertNOAA_IOAS_Zooplankton1();
    void doConvertNOAA_IOAS_Zooplankton2();
    void doConvertNOAA_IOAS_Zooplankton3();
    void doConvertNOAA_IOAS_Zooplankton4();

    void doConvertTSG();
    void doCreateGpxFile();
    void doCreateKmlFile();
    void doCliwocConverter();
    void doConvertSOCAT();
    void doWOCEWHPConverter();

// Dialogs
    int  doGeneralOptionsDialog();

private:
    QStringList expandCommandline();
    QStringList renameFiles( const QStringList &Filename, const QString &searchStr, const QString &replaceStr );
    bool buildFilename( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QString &Filename, QString &FilenameIn, QString &FilenameOut );
    bool containsBinaryFile( const QStringList &FilenameList );
    bool existsFirstFile( const int ActionNumber, const QString &FilenameFormat, const int Extension, QStringList &FilenameList );
    bool isEmptyLine( const QString &String );
    QString findCurl();
    QString findZip( const int mode = 1 );
    QString findUnzip( int const mode = 1 );
    int NumOfSections( const QString &String );
    int addToFilenameList( QStringList &FilenameList, const QString &Filename, const bool showAllFiles = true, const bool decompressFile = false );
    int calcFileSizeClass( const QString &FilenameIn, const int NumOfFiles, const int lowerLimit = 100, const int upperLimit = 1000 );
    int copyStringList( const QStringList &InStringList, QStringList &OutStringList, const QString &s_Pattern = "*" );
    int emptyDir( const QString &Dir );
    int decompressFile( const QString &Filename, const bool delZipFile = false, const int mode = 1, const QString &Program = "zip" );
    int incFileProgress( const int NumOfFiles, const int FileNumber );
    int incProgress( const int NumOfFiles, const int Step );
    int readFile( const QString &FilenameIn, QStringList &Input, const int Codec = -1, const int NumOfFiles = 0, const qint64 Bytes = 0 );
    int removeFile( const QString &Filename );
    int downloadFile( const QString &Curl, const QString &Url, const QString &Filename );
    unsigned int incProgress( const int NumOfFiles, const unsigned int filesize, const unsigned int length, const QString &InputStr );
    void appendItem( QStringList &List, const QString &Item, const QString &SS = "", const QString &RS = "" );
    void compressFolder( const QString &FolderIn, const int mode = 1, const QString &Program = "zip" );
    void compressFile( const QString &FilenameIn, const int mode = 1, const QString &Program = "zip" );
    void clearFilenameList( int &ActionNumber, QStringList &FilenameList );
    void clearList( QStringList &List );
    void clearMessage();
    void createActions();
    void createMenus();
    void createStatusBar( const bool showProgressBar );
    void enableMenuItems( const QStringList &FilenameList );
    void endTool( const int err, const int stopProgress, int &ActionNumber, const QString &FilenameFormat, const int Extension, QStringList &FilenameList, const QString &doneMessage = "Done", const QString &canceledMessage = "Converter was canceled", const bool clearList = false, const bool incActionNumber = true );
    void initFileProgress( const int NumOfFiles, const QString &FilenameIn, const QString &MessageText );
    void initProgress( const int NumOfFiles, const QString &Filename, const QString &MessageText, const int totalNumberOfSteps );
    void initProgress( const int totalNumberOfSteps );
    void listDir( QStringList &Filename, const QString &Directory, const bool showAllFiles = true, const bool decompressFile = false );
    void loadPreferences();
    void resetFileProgress( const int NumOfFiles );
    void resetProgress( const int NumOfFiles = 0 );
    void savePreferences();
    void setNormalCursor();
    void setStatusBar( const QString &Message = "", const int seconds = 0 );
    void setStatusBarFileInProgress( const QString &Filename, const QString &Message = "", const int i_seconds = 0 );
    void setWTitle( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QStringList &FilenameList );
    void setWaitCursor();
    void showFilenameList( const int ActionNumber, const QString &FilenameFormat, const int Extension, const QStringList &FilenameList );
    void showList( const QStringList &List );
    void showMessage( const QString &Message, QStringList &MessageList );
    void wait( const int msec );

    QString getApplicationName( const bool replaceUnderline = false );
    QString getDataLocation();
    QString getDocumentDir();
    QString getPreferenceFilename();
    QString getVersion();
    QString setEOLChar( const int EOL );
    QString setExtension( const int Extension );
    QString getFileChecksum( const QString &FilenameIn );

    QMenu   *fileMenu;
    QMenu   *converterMenu;
    QMenu   *helpMenu;

    QAction *newWindowAction;
    QAction *openFileAction;
    QAction *openFolderAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *hideWindowAction;
    QAction *setOptionsAction;
    QAction *exitAction;
    QAction *getFilesAction;

    QAction *compressFilesAction;
    QAction *convertSOCATAction;
    QAction *convertCLIWOCAction;
    QAction *convertCHUAN_stationAction;
    QAction *convertCHUAN_movingAction;
    QAction *convertMastertrackAction;
    QAction *createMastertrackImportFile30secAction;
    QAction *createMastertrackImportFile1minAction;
    QAction *createMastertrackImportFile10minAction;

    QAction *convertNOAA_IOAS_BENTHOS_Action;
    QAction *convertNOAA_IOAS_HYDROLOGY_Action;
    QAction *convertNOAA_IOAS_METEOROLOGY_Action;
    QAction *convertNOAA_IOAS_PLANKTON_Action;
    QAction *convertNOAA_IOAS_PHYTOPLANKTON1_Action;
    QAction *convertNOAA_IOAS_PHYTOPLANKTON2_Action;
    QAction *convertNOAA_IOAS_ZOOPLANKTON1_Action;
    QAction *convertNOAA_IOAS_ZOOPLANKTON2_Action;
    QAction *convertNOAA_IOAS_ZOOPLANKTON3_Action;
    QAction *convertNOAA_IOAS_ZOOPLANKTON4_Action;

    QAction *convertTSGAction;
    QAction *createGpxFileAction;
    QAction *createKmlFileAction;
    QAction *parseCrossRefXMLAction;
    QAction *parseTreeRingXMLAction;

    QAction *aboutAction;
    QAction *aboutQtAction;
    QAction *showHelpAction;

    QProgressDialog *ProgressDialog;
    QProgressDialog *FileProgressDialog;
    QProgressBar    *ProgressBar;
    QLabel          *StatusMessage;

    QStringListModel    *Model;
    QListView           *ListView;
    QTextEdit           *textViewer;

    QPoint	posWindow;              //!< Position des Programmfensters.
    QSize	sizeWindow;             //!< Groesse des Programmfensters.

    QPoint	posDialog;              //!< Position der Dialoge.
    QSize	sizeParameterDialog;    //!< Groesse der Parameterdialogs.
};

#endif
