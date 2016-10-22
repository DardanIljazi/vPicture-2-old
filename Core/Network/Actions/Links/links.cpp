#include "../../network.h"

void Network::getImagesWithDate(int minday){
    QVariantMap arguments;
    arguments["days"] = minday;

    if(!localServerData->getValue("id").toString().isEmpty())
        arguments["account"] = localServerData->getValue("id").toString();

    localUrl->url("getimageswithdate", arguments);

    get(localUrl->getUrl());
}
