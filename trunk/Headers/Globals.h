#ifndef _GLOBALS

    #define _GLOBALS

// Application constants
    const QString _PROGRAMDOI_                       = "doi:10.1594/PANGAEA.823690";
    const QString _PROGRAMREADME_                    = "http://wiki.pangaea.de/wiki/PanConverter";

    const QString _VERSION_                          = "1.6";	// Version number, 2015-02-19
    const QChar   _PSEPARATOR_                       = '|';

    const int   _NOERROR_                            = 0;
    const int   _ERROR_                              = 1;
    const int   _APPBREAK_                           = 2;
    const int   _NODATAFOUND_                        = 3;
    const int   _DATAFOUND_                          = 4;
    const int   _CHOOSEABORTED_                      = 5;
    const int   _FILENOTEXISTS_                      = 6;
    const int   _ZIP_                                = 1;
    const int   _GZIP_                               = 2;

    const int   _TABSTOP_                            = 0;
    const int   _COMMA_                              = 1;
    const int   _SEMICOLON_                          = 2;
    const int   _SPACE_                              = 3;
    const int   _DOT_                                = 4;

    const int   _HYDROLOGY_                          = 0;
    const int   _PHYTOPLANKTON1_                     = 11;
    const int   _PHYTOPLANKTON2_                     = 12;
    const int   _ZOOPLANKTON1_                       = 21;
    const int   _ZOOPLANKTON2_                       = 22;
    const int   _ZOOPLANKTON3_                       = 23;
    const int   _ZOOPLANKTON4_                       = 24;
    const int   _METEOROLOGY_                        = 31;
    const int   _BENTHOS_                            = 41;
    const int   _PLANKTON_                           = 51;

// EOL
    const int   _UNIX_                               = 0;    // LF
    const int   _MACOS_                              = 1;    // CR
    const int   _WIN_                                = 2;    // CR+LF

// Encoding
    const int   _SYSTEM_                             = -1;   // System
    const int   _UTF8_                               = 0;    // UTF-8
    const int   _APPLEROMAN_                         = 1;    // Apple Roman
    const int   _LATIN1_                             = 2;    // Latin-1 = ISO 8859-1

// Color
    const int _RED_                                  = 0;
    const int _GREEN_                                = 1;
    const int _BLUE_                                 = 2;
    const int _ORANGE_                               = 3;
    const int _YELLOW_                               = 4;

// Extension
    const int   _TXT_                                = 0;
    const int   _CSV_                                = 1;

#endif
