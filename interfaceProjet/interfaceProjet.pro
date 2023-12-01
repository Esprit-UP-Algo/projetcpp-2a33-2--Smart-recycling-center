QT += core gui quick widgets
QT += location
QT += charts

QT       += core gui  serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets quickwidgets
CONFIG += c++11



QT += sql

CONFIG += console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    activitydialog.cpp \
    arduino.cpp \
    connect.cpp \
    main.cpp \
    mainwindow.cpp \
    pointdecollect.cpp

HEADERS += \
    activitydialog.h \
    arduino.h \
    connect.h \
    mainwindow.h \
    pointdecollect.h

FORMS += \
    activitydialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    logo.qrc \
    qml.qrc


DISTFILES += \
    D:/Users/HP/Downloads/379629718_1462667831247521_2760304968014038212_n.png \
    D:/Users/HP/Downloads/Chap0_Nouveautés_2022 (1).pptx \
    D:/Users/HP/Downloads/Chap0_Nouveautés_2022.pptx \
    D:/Users/HP/Downloads/Chapitre 1- POO-MAJ_V2.pptx \
    D:/Users/HP/Downloads/Chapitre 2- Encapsulation.pptx \
    D:/Users/HP/Downloads/Chapitre 3- Constructeur_Destructeur_2122.pptx \
    D:/Users/HP/Downloads/Cours_de_C-Cpppar_Christian_Casteyde.pdf \
    D:/Users/HP/Downloads/Guide utilisation Moodle.docx \
    D:/Users/HP/Downloads/Journal de Stage stages obligatoires.pdf \
    D:/Users/HP/Downloads/NordVPNSetup.exe \
    D:/Users/HP/Downloads/Rapport-de-stage.pdf \
    D:/Users/HP/Downloads/Stage_1ere_annee-3.pdf \
    D:/Users/HP/Downloads/Stages_Obligatoires_Esprit.pdf \
    D:/Users/HP/Downloads/esprit-tn.com_ESPOnline_Etudiants_demande_de_stage.aspx.pdf \
    D:/Users/HP/Downloads/gemstone-recycle-logo.png \
    D:/Users/HP/Downloads/letter daff (1).pdf \
    D:/Users/HP/Downloads/letter daff.pdf \
    D:/Users/HP/Downloads/qt-opensource-windows-x86-5.9.9.exe \
    D:/Users/HP/Downloads/rapport de stage (1).docx \
    D:/Users/HP/Downloads/rapport de stage (2).docx \
    D:/Users/HP/Downloads/rapport de stage.docx \
    D:/Users/HP/Downloads/rapport de stage.pdf \
    D:/Users/HP/Downloads/utweb_installer.exe
