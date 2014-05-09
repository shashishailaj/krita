#include "KoBundleCreationWidget.h"
#include "ui_KoBundleCreationWidget.h"

#include "KoXmlResourceBundleMeta.h"

#include <QProcessEnvironment>
#include <QFileInfo>

#include <resourcemanager.h>

KoBundleCreationWidget::KoBundleCreationWidget(KoXmlResourceBundleMeta* newMeta, QWidget *parent)
    : KDialog(parent)
    , m_ui(new Ui::KoBundleCreationWidget)
    , m_newMeta(newMeta)
{
    m_page = new QWidget();
    m_ui->setupUi(m_page);
    setMainWidget(m_page);
    resize(m_page->sizeHint());
    setButtons(Ok | Cancel);
    setDefaultButton(Ok);

    connect(this, SIGNAL(okClicked()), SLOT(createBundle()));
    connect(this, SIGNAL(cancelClicked()), SLOT(reject()));
}

KoBundleCreationWidget::~KoBundleCreationWidget()
{
    delete m_ui;
}

//TODO Vérifier la présence de caractères invalides dans le nom du paquet (exemple : *"')
//Même s'ils semblent acceptés par le système
void KoBundleCreationWidget::createBundle()
{
    QString name = m_ui->editBundleName->text().remove(" ");

    if (name.isEmpty()) {
        m_ui->editBundleName->setStyleSheet(QString(" border: 1px solid red"));
        emit status("Empty bundle name...");
    }
    else {

        KoResourceServer<KoResourceBundle> *rServer = ResourceBundleServerProvider::instance()->resourceBundleServer();

        QFileInfo fileInfo(rServer->saveLocation() + name + ".bundle");

        if (fileInfo.exists()) {
            m_ui->editBundleName->setStyleSheet("border: 1px solid red");
            emit status("Bundle already exists : choose another name...");
        }
        else {
            m_newMeta->setMeta(name, m_ui->editAuthor->text(), m_ui->editLicense->text(),
                               m_ui->editWebsite->text(),
                               m_ui->editDescription->document()->toPlainText());
            accept();
        }
    }
}


