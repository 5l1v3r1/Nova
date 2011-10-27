//============================================================================
// Name        : novagui.h
// Author      : DataSoft Corporation
// Copyright   : GNU GPL v3
// Description : Header file for the main NovaGUI component
//============================================================================/*

#ifndef NOVAGUI_H
#define NOVAGUI_H

#include <QtGui/QMainWindow>
#include "ui_novagui.h"
#include <tr1/unordered_map>
#include <sys/un.h>
#include <arpa/inet.h>
#include <Suspect.h>

///	Filename of the file to be used as an Classification Engine IPC key
#define CE_FILENAME "/etc/CEKey"

///The maximum message, as defined in /proc/sys/kernel/msgmax
#define MAX_MSG_SIZE 65535
//Number of messages to queue in a listening socket before ignoring requests until the queue is open
#define SOCKET_QUEUE_SIZE 50

class NovaGUI : public QMainWindow
{
    Q_OBJECT

public:

    NovaGUI(QWidget *parent = 0);
    ~NovaGUI();
    Ui::NovaGUIClass ui;

    ///Receive a input from Classification Engine.
    bool ReceiveCE(int socket);

    ///Processes the recieved suspect in the suspect table
    void updateSuspect(Nova::ClassificationEngine::Suspect* suspect);

    ///Updates the UI with the latest suspect information
    void drawSuspects();

private slots:

	void on_SuspectButton_clicked();

	void on_CEButton_clicked();
	void on_CELoadButton_clicked();
	void on_CESaveButton_clicked();

	void on_DMButton_clicked();
	void on_DMLoadButton_clicked();
	void on_DMSaveButton_clicked();

	void on_HSButton_clicked();
	void on_HSLoadButton_clicked();
	void on_HSSaveButton_clicked();

	void on_LTMButton_clicked();
    void on_LTMLoadButton_clicked();
    void on_LTMSaveButton_clicked();


private:

};

using namespace Nova;
using namespace ClassificationEngine;

typedef std::tr1::unordered_map<in_addr_t, Suspect*> SuspectHashTable;

/// This is a blocking function. If nothing is received, then wait on this thread for an answer
void *CEListen(void *ptr);
/// Updates the suspect list every so often.
void *CEDraw(void *ptr);

///Socket closing workaround for namespace issue.
void sclose(int sock);

//Opens the socket and creates a thread to listen on it.
void openSocket(NovaGUI *window);



#endif // NOVAGUI_H
