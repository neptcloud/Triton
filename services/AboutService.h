/******************************************************************************
 * Copyright (c) 2013 - 2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#ifndef _ABOUTSERVICE_H_
#define _ABOUTSERVICE_H_

/** @defgroup AboutService
 *
 *  @{
 */

#include "AboutIcon.h"
#include "PropertyStore.h"
#include "Services_Common.h"

/**
 * published About feature objects and interfaces
 */
extern const AJ_InterfaceDescription AJ_AboutInterfaces[];

/*
 * Following definitions are read by the application.
 */

#define NUM_ABOUT_OBJECTS (1 + NUM_ABOUT_ICON_OBJECTS)

#define ABOUT_APPOBJECTS \
    { "/About",              AJ_AboutInterfaces }, \
    ABOUT_ICON_APPOBJECTS

#define ABOUT_ANNOUNCEOBJECTS ABOUT_APPOBJECTS

/*
 * About Feature API
 */

/**
 * start the About feature passing relevant initialization parameters
 * @param servicePort
 * @param announceObjects
 * @return status
 */
AJ_Status AJ_About_Start(uint16_t servicePort, AJ_Object* announceObjects);

/**
 * handler for property getters associated with org.alljoyn.About
 * @param replyMsg
 * @param propId
 * @param context
 * @return aj_status
 */
AJ_Status AJ_About_PropGetHandler(AJ_Message* replyMsg, uint32_t propId, void* context);

/**
 * handler for property setters associated with org.alljoyn.About
 * @param replyMsg
 * @param propId
 * @param context
 * @return aj_status
 */
AJ_Status AJ_About_PropSetHandler(AJ_Message* replyMsg, uint32_t propId, void* context);

/**
 * handler for GetAboutData request in org.alljoyn.About
 * @param msg
 * @return aj_status
 */
AJ_Status AJ_About_GetAboutData(AJ_Message* msg);

/**
 * handler for GetObjectDescription request in org.alljoyn.About
 * @param replyMsg
 * @param propId
 * @param context
 * @return aj_status
 */
AJ_Status AJ_About_GetObjectDescription(AJ_Message* msg);

/**
 * send Announce signal in org.alljoyn.About
 * @param bus
 * @return aj_status
 */
AJ_Status AJ_About_Announce(AJ_BusAttachment* busAttachment);

/**
 * check if should send announcement
 * @return boolean
 */
uint8_t AJ_About_IsShouldAnnounce();

/**
 * set whether should send announcement
 * @param boolean
 */
void AJ_About_SetShouldAnnounce(uint8_t shouldAnnounce);

/**
 * connected handler
 * @param bus
 * @return status
 */
AJ_Status AJ_About_ConnectedHandler(AJ_BusAttachment* busAttachment);

/**
 * message processing
 * @param bus
 * @param msg
 * @return status
 */
AJSVC_ServiceStatus AJ_About_MessageProcessor(AJ_BusAttachment* busAttachment, AJ_Message* msg, AJ_Status* msgStatus);

/**
 * disconnect handler
 * @param bus
 * @return status
 */
AJ_Status AJ_About_DisconnectHandler(AJ_BusAttachment* busAttachment);

/** @} */
 #endif // _ABOUTSERVICE_H_
