#include "argumentsparser.h"

ArgumentsParser::ArgumentsParser(QCoreApplication *app)
{
    // -abc is interpreted as --abc
    setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);

    setApplicationDescription("vPicture App is a free picture uploader by CogDev Team");
    addHelpOption();
    addVersionOption();

    process(*app);
}
