#include "argumentsparser.h"


ArgumentsParser::ArgumentsParser(QStringList arguments)
{
    lastArguments = arguments;
}

bool ArgumentsParser::parseArguments(){
    if(lastArguments.count() > 0){
        for(int i = 0; i < lastArguments.count(); i ++){
            if(lastArguments.at(i).contains(HideWindows))
                emit hideWindows();
            else if(lastArguments.at(i).contains(ShowWindows))
                emit showWindows();

            if(lastArguments.at(i).contains(NoEnumWindows))
                emit noEnumWindows();

            if(lastArguments.at(i).contains(Connect)){ //connect %{user} %{password}
                emit connectUser(lastArguments.at(i+1), lastArguments.at(i+2));
                i+= 2;
            }

            if(lastArguments.at(i).contains(NoKeyEvent)){
                emit noKeyEvent();
            }

        }
    }

    return false;
}


