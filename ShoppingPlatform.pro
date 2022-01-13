QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    author_code_dialog.cpp \
    changepwd_dialog.cpp \
    cpp/addgoods_list.cpp \
    cpp/addgoods_window.cpp \
    cpp/card.cpp \
    cpp/carddetail_window.cpp \
    cpp/csv.cpp \
    cpp/loading_window.cpp \
    cpp/managegoods_window.cpp \
    cpp/manage_list.cpp \
    cpp/shop_window.cpp \
    cpp/shop_list.cpp \
    customer.cpp \
    customerbag.cpp \
    double_check_dialog.cpp \
    exchange_popup.cpp \
    loginwindowpopupform.cpp \
    main.cpp \
    mainwindow.cpp \
    manager.cpp \
    money.cpp \
    person.cpp \
    piechartwidget.cpp \
    seller.cpp

HEADERS += \
    author_code_dialog.h \
    changepwd_dialog.h \
    customer.h \
    customerbag.h \
    double_check_dialog.h \
    exchange_popup.h \
    header/addgoods_list.h \
    header/addgoods_window.h \
    header/card.h \
    header/carddetail_window.h \
    header/csv.h \
    header/loading_window.h \
    header/shop_window.h \
    header/shop_list.h \
    header/managegoods_window.h \
    header/manage_list.h \
    loginwindowpopupform.h \
    mainwindow.h \
    manager.h \
    money.h \
    person.h \
    piechartwidget.h \
    seller.h

FORMS += \
    author_code_dialog.ui \
    changepwd_dialog.ui \
    double_check_dialog.ui \
    exchange_popup.ui \
    ui/addgoods_list.ui \
    ui/managegoods_window.ui \
    ui/manage_list.ui \
    loginwindowpopupform.ui \
    mainwindow.ui \
    ui/addgoods_window.ui \
    ui/carddetail_window.ui \
    ui/loading_window.ui \
    ui/shop_window.ui \
    ui/shop_list.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
