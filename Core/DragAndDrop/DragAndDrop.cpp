#include "../mainwindow.h"


void MainWindow::selectFileToUpload()
{
    QList<QUrl> files = QFileDialog::getOpenFileUrls(this, tr("Select your file(s)"), settings->getValue("lastDir").toString(), config->mimeDataAllowedScheme());

    // We save the lastDir to the settings (lastDir is the last folder where the user was)
    if(files.count() > 0){
        settings->setValue("lastDir", QFileInfo(files.at(0).toString()).absoluteFilePath());
        initializeUrls(files);
        uploadFiles();
    }
}

bool MainWindow::initializeUrls(QList<QUrl> urls){
    QList<QUrl> urlsList = urls;
    if(urlsList.count() > 0){
        for(int i = 0; i < urlsList.count(); i++)
        {
            // Verification ==> if the extension is allowed in Config class (getListMimeDataAllowed)
            if(config->getListMimeDataAllowed().
                    indexOf( QFileInfo(urlsList.at(i).toLocalFile() ).suffix().toLower() ) == -1){
                // Error, the extension is not allowed

                // Show the error in Notification class
                notification->addNotification(Notification::Errors_Notif, errors->getLastError());

                // The url is not allowed and is suppressed from the urls list
                urlsList.removeAt(i);
            }
        }
        // Set the QList of urls into the Files Class
        files->setUrls(urlsList);

        // If the urlList is now empty, that means that the file sent was not an authorized format
        if(urlsList.isEmpty()){
            changeMiddlePictureState(MainWindow::Invalid_Files);
            return false;
        }else{
            changeMiddlePictureState(MainWindow::Valid_Files);
            return true;
        }
    }else
        return false;

}

void MainWindow::dragEnterEvent(QDragEnterEvent *event){
    if(event->mimeData()->hasUrls()){
        if(initializeUrls(event->mimeData()->urls())){
            // Accept the drag event. This "accept" call the dropEvent() function
            event->acceptProposedAction();
            event->accept();
        }
    }
}

void MainWindow::uploadFiles(){
    // Show progress bar
    changeProgressBarProgression(0, 0);

    // We upload the files by getting the urls from Files class
    upload->upload(files->getUrls());
}

// After the drag event is accepted (with event->accept and event->acceptProposedAction), it calls the dropEvent function
void MainWindow::dropEvent(QDropEvent *){
    // We initialize design by default (because the user could have soon uploaded a picture)
    initDesignElements();
    uploadFiles();
}

