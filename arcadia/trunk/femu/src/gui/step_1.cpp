#include "step_1.h"
#include "step_2.h"
#include <util/singleton.h>

UI_Step1::UI_Step1(QWidget *parent): QWidget(parent) {
    ui = new Ui::UI_Step1();
    ui->setupUi(this);

    move(QApplication::desktop()->screen()->rect().center() - rect().center());

    int n = 3, m = 2;

    svgWidgets.resize(n * m);
    for (int i = 0; i < n * m; i++) {
        svgWidgets[i] = new QSvgWidget(this);
        ui->interactionVertexes->addWidget(svgWidgets[i], i / m, i % m);
    }

    ImportTheories();
    ShowTheories();

    QObject::connect(ui->theories, SIGNAL(currentTextChanged(QString)), this, SLOT(HandleClick()));
    QObject::connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(Next()));
}

UI_Step1::~UI_Step1() {
    delete ui;
}

void UI_Step1::ImportTheories() {
    QDir current(QApplication::applicationDirPath());
    if (!current.cd("theories")) {
        QMessageBox msgBox;
        msgBox.setText("No theories/ dir found in " + QApplication::applicationDirPath());
        msgBox.exec();
        exit(0);
    }
    QStringList xmls = current.entryList(QStringList() << "*.xml");
    for (QStringList::ConstIterator i = xmls.constBegin(); i != xmls.constEnd(); i++) {
        TFeynRules *rules = new TFeynRules();
        rules->ImportFromXml(current.absoluteFilePath(*i));
        theories.insert(rules->Name, rules);
    }
}

void UI_Step1::ShowTheories() {
    ui->theories->clear();
    for (QHash<QString, TFeynRules*>::ConstIterator i = theories.begin();
         i != theories.end(); i++) {
        ui->theories->addItem(i.key());
    }
}

void UI_Step1::HandleClick() {
    TFeynRules *rules = theories.value(ui->theories->currentItem()->text());
    currentTheory = rules;
    QVector<QByteArray> svgData;
    for (QSet<TInteraction*>::ConstIterator i = rules->Interactions.begin();
         i != rules->Interactions.end(); i++) {
        TDiagram d;
        (*i)->GenerateExampleDiagram(&d);
        svgData << d.GenerateImageData((*i)->Name);
    }

    for (int i = 0; i < svgWidgets.size(); i++) {
        if (i < svgData.size()) {
            svgWidgets[i]->load(svgData[i]);
        } else
            svgWidgets[i]->load(QByteArray());
    }
}

void UI_Step1::Next() {
    hide();
    UI_Step2 *s2 = new UI_Step2();
    TSingleton<UI_Step2>::Switch(s2);
    s2->Initialize(currentTheory);
    s2->show();
}
