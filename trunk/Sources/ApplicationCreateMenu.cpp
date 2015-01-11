// ***********************************************************************************************
// *                                                                                             *
// * createMenu.cpp - creates application menus                                                  *
// *                                                                                             *
// * Dr. Rainer Sieger - 2008-05-18                                                              *
// *                                                                                             *
// ***********************************************************************************************

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Erstellen der Menue-Aktionen. */

void MainWindow::createActions()
{
// File menu
    newWindowAction = new QAction(trUtf8("&New window"), this);
    newWindowAction->setShortcut(trUtf8("Ctrl+N"));
    connect(newWindowAction, SIGNAL(triggered()), this, SLOT(newWindow()));

    openFileAction = new QAction(tr("&Open..."), this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(chooseFiles()));

    openFolderAction = new QAction(tr("Select &Folder..."), this);
    openFolderAction->setShortcut(tr("Ctrl+F"));
    connect(openFolderAction, SIGNAL(triggered()), this, SLOT(chooseFolder()));

    saveAction = new QAction(trUtf8("&Save"), this);
    saveAction->setShortcut(trUtf8("Ctrl+S"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAction = new QAction(trUtf8("Save &as..."), this);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    compressFilesAction = new QAction(trUtf8("Compress files with zip..."), this);
    connect(compressFilesAction, SIGNAL(triggered()), this, SLOT(doCompressFiles()));

    hideWindowAction = new QAction(tr("&Close window"), this);
    hideWindowAction->setShortcut(tr("Ctrl+W"));
    connect(hideWindowAction, SIGNAL(triggered()), this, SLOT(hideWindow()));

    setOptionsAction = new QAction(trUtf8("General o&ptions"), this);
    setOptionsAction->setShortcut(trUtf8("Ctrl+P"));
    connect(setOptionsAction, SIGNAL(triggered()), this, SLOT(doGeneralOptionsDialog()));

    exitAction = new QAction(trUtf8("&Quit"), this);
    exitAction->setShortcut(trUtf8("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));


    // converter

    convertCHUAN_movingAction = new QAction(trUtf8("Comprehensive Historical Upper-Air Network (CHUAN), moving"), this);
    connect(convertCHUAN_movingAction, SIGNAL(triggered()), this, SLOT(doConvertCHUAN_moving()));

    convertCHUAN_stationAction = new QAction(trUtf8("Comprehensive Historical Upper-Air Network (CHUAN), station"), this);
    connect(convertCHUAN_stationAction, SIGNAL(triggered()), this, SLOT(doConvertCHUAN_station()));

    convertCLIWOCAction = new QAction(trUtf8("International Maritime Meteorological Archive (IMMA)"), this);
    connect(convertCLIWOCAction, SIGNAL(triggered()), this, SLOT(doCliwocConverter()));

    convertSOCATAction = new QAction(trUtf8("Surface Ocean CO2 Atlas (SOCAT)"), this);
    connect(convertSOCATAction, SIGNAL(triggered()), this, SLOT(doConvertSOCAT()));

    convertNOAA_IOAS_BENTHOS_Action = new QAction(trUtf8("International Ocean Atlas Series (IOAS) - Benthos"), this);
    connect(convertNOAA_IOAS_BENTHOS_Action, SIGNAL(triggered()), this, SLOT(doConvertNOAA_IOAS_Benthos()));

    convertNOAA_IOAS_HYDROLOGY_Action = new QAction(trUtf8("International Ocean Atlas Series (IOAS) - Hydrology"), this);
    connect(convertNOAA_IOAS_HYDROLOGY_Action, SIGNAL(triggered()), this, SLOT(doConvertNOAA_IOAS_Hydrology()));

    convertNOAA_IOAS_METEOROLOGY_Action = new QAction(trUtf8("International Ocean Atlas Series (IOAS) - Meteorology"), this);
    connect(convertNOAA_IOAS_METEOROLOGY_Action, SIGNAL(triggered()), this, SLOT(doConvertNOAA_IOAS_Meteorology()));

    convertNOAA_IOAS_PLANKTON_Action = new QAction(trUtf8("International Ocean Atlas Series (IOAS) - Plankton []"), this);
    connect(convertNOAA_IOAS_PLANKTON_Action, SIGNAL(triggered()), this, SLOT(doConvertNOAA_IOAS_Plankton()));

    convertNOAA_IOAS_PHYTOPLANKTON1_Action = new QAction(trUtf8("International Ocean Atlas Series (IOAS) - Phytoplankton [#/l]"), this);
    connect(convertNOAA_IOAS_PHYTOPLANKTON1_Action, SIGNAL(triggered()), this, SLOT(doConvertNOAA_IOAS_Phytoplankton1()));

    convertNOAA_IOAS_PHYTOPLANKTON2_Action = new QAction(trUtf8("International Ocean Atlas Series (IOAS) - Phytoplankton []"), this);
    connect(convertNOAA_IOAS_PHYTOPLANKTON2_Action, SIGNAL(triggered()), this, SLOT(doConvertNOAA_IOAS_Phytoplankton2()));

    convertNOAA_IOAS_ZOOPLANKTON1_Action = new QAction(trUtf8("International Ocean Atlas Series (IOAS) - Zooplankton [#]"), this);
    connect(convertNOAA_IOAS_ZOOPLANKTON1_Action, SIGNAL(triggered()), this, SLOT(doConvertNOAA_IOAS_Zooplankton1()));

    convertNOAA_IOAS_ZOOPLANKTON2_Action = new QAction(trUtf8("International Ocean Atlas Series (IOAS) - Zooplankton [#/m**3]"), this);
    connect(convertNOAA_IOAS_ZOOPLANKTON2_Action, SIGNAL(triggered()), this, SLOT(doConvertNOAA_IOAS_Zooplankton2()));

    convertNOAA_IOAS_ZOOPLANKTON3_Action = new QAction(trUtf8("International Ocean Atlas Series (IOAS) - Zooplankton [mg]"), this);
    connect(convertNOAA_IOAS_ZOOPLANKTON3_Action, SIGNAL(triggered()), this, SLOT(doConvertNOAA_IOAS_Zooplankton3()));

    convertNOAA_IOAS_ZOOPLANKTON4_Action = new QAction(trUtf8("International Ocean Atlas Series (IOAS) - Zooplankton []"), this);
    connect(convertNOAA_IOAS_ZOOPLANKTON4_Action, SIGNAL(triggered()), this, SLOT(doConvertNOAA_IOAS_Zooplankton4()));

    parseCrossRefXMLAction = new QAction(trUtf8("CrossRef XML file(s)"), this);
    connect(parseCrossRefXMLAction, SIGNAL(triggered()), this, SLOT(doParseCrossRefXML()));

    parseTreeRingXMLAction = new QAction(trUtf8("Tree Ring Data Standard (TRiDaS)"), this);
    connect(parseTreeRingXMLAction, SIGNAL(triggered()), this, SLOT(doParseTreeRingXML()));

    convertMastertrackAction = new QAction(trUtf8("Mastertrack (convert FIELAX -> PANGAEA)"), this);
    connect(convertMastertrackAction, SIGNAL(triggered()), this, SLOT(doConvertMastertrack()));

    createMastertrackImportFileAction = new QAction(trUtf8("Mastertrack (create import file)"), this);
    connect(createMastertrackImportFileAction, SIGNAL(triggered()), this, SLOT(doCreateMastertrackImportFile()));

    convertTSGAction = new QAction(trUtf8("Thermosalinograph (TSG)"), this);
    connect(convertTSGAction, SIGNAL(triggered()), this, SLOT(doConvertTSG()));

    createGpxFileAction = new QAction(tr("Columus GPS Data Logger V-900 -> GPX..."), this);
    connect(createGpxFileAction, SIGNAL(triggered()), this, SLOT(doCreateGpxFile()));

    createKmlFileAction = new QAction(tr("Columus GPS Data Logger V-900 -> KML..."), this);
    connect(createKmlFileAction, SIGNAL(triggered()), this, SLOT(doCreateKmlFile()));

    // Help menu
    aboutAction = new QAction(trUtf8("&About ") + getApplicationName( true ), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(trUtf8("About &Qt"), this);
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    showHelpAction = new QAction(getApplicationName( true ) + trUtf8(" &Help"), this);
    showHelpAction->setShortcut(trUtf8("F1"));
    connect(showHelpAction, SIGNAL(triggered()), this, SLOT(displayHelp()));

}

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************

/*! @brief Verbindet Menues mit Aktionen. */

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( trUtf8( "&File" ) );
    fileMenu->addAction( openFileAction );
    fileMenu->addAction( openFolderAction );
    fileMenu->addSeparator();
    fileMenu->addAction( compressFilesAction );
    fileMenu->addSeparator();

#if defined(Q_OS_LINUX)
    ;
#endif

#if defined(Q_OS_MAC)
    fileMenu->addAction( newWindowAction );
    newWindowAction->setEnabled( false );
    fileMenu->addAction( hideWindowAction );
#endif

#if defined(Q_OS_WIN)
    fileMenu->addAction( hideWindowAction );
#endif

    fileMenu->addSeparator();
    fileMenu->addAction( setOptionsAction );

#if defined(Q_OS_LINUX)
    fileMenu->addSeparator();
    fileMenu->addAction( exitAction );
#endif

#if defined(Q_OS_WIN)
    fileMenu->addSeparator();
    fileMenu->addAction( exitAction );
#endif

// **********************************************************************************************

    converterMenu = menuBar()->addMenu( trUtf8( "Converter" ) );

    converterMenu->addAction( parseCrossRefXMLAction );
    converterMenu->addSeparator();
    converterMenu->addAction( parseTreeRingXMLAction );
    converterMenu->addSeparator();
    converterMenu->addAction( convertCLIWOCAction );
    converterMenu->addAction( convertSOCATAction );

    converterMenu->addSeparator();
    converterMenu->addAction( convertNOAA_IOAS_BENTHOS_Action );
    converterMenu->addAction( convertNOAA_IOAS_HYDROLOGY_Action );
    converterMenu->addAction( convertNOAA_IOAS_METEOROLOGY_Action );
    converterMenu->addAction( convertNOAA_IOAS_PLANKTON_Action );
    converterMenu->addAction( convertNOAA_IOAS_PHYTOPLANKTON1_Action );
    converterMenu->addAction( convertNOAA_IOAS_PHYTOPLANKTON2_Action );
    converterMenu->addAction( convertNOAA_IOAS_ZOOPLANKTON1_Action );
    converterMenu->addAction( convertNOAA_IOAS_ZOOPLANKTON2_Action );
    converterMenu->addAction( convertNOAA_IOAS_ZOOPLANKTON3_Action );
    converterMenu->addAction( convertNOAA_IOAS_ZOOPLANKTON4_Action );
    converterMenu->addSeparator();
    converterMenu->addAction( convertCHUAN_movingAction );
    converterMenu->addAction( convertCHUAN_stationAction );
    converterMenu->addSeparator();
    converterMenu->addAction( convertMastertrackAction );
    converterMenu->addAction( createMastertrackImportFileAction );
    converterMenu->addSeparator();
    converterMenu->addAction( convertTSGAction );
    converterMenu->addSeparator();
    converterMenu->addAction( createGpxFileAction );
    converterMenu->addAction( createKmlFileAction );

// **********************************************************************************************

    helpMenu = menuBar()->addMenu( trUtf8( "&Help" ) );
    helpMenu->addAction( aboutAction );
    helpMenu->addAction( aboutQtAction );
    helpMenu->addSeparator();
    helpMenu->addAction( showHelpAction );
}
