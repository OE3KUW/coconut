#ifndef MBXI_IF_H
#define MBXI_IF_H

#include "bpci_if.h"

typedef UCHAR MAILBOX;
typedef UCHAR MAIL;

#define NULL_MBX ((MAILBOX)ZERO)

#ifdef CNF_ASSERTING
#define MBXI_Create(name) mMBXI_Create(name)
#else
#define MBXI_Create(name) mMBXI_Create()
#endif

void MBXI_Init(void);
#ifdef CNF_ASSERTING
MAILBOX mMBXI_Create(STRING name);
#else
MAILBOX mMBXI_Create(void);
#endif
void MBXI_Connect(MAILBOX mailbox, TASK task);
void MBXI_Disconnect(MAILBOX mailbox, TASK task);
BOOL MBXI_Send(MAILBOX mailbox, MAIL value);
UCHAR MBXI_Count(MAILBOX mailbox);
BOOL MBXI_Receive(MAILBOX mailbox, MAIL *value);

#endif
