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

#include <alljoyn.h>
#include "AboutService.h"

#define CHECK(x) if ((status = (x)) != AJ_OK) { break; }

/*
 * Message identifiers for the method calls this service implements
 */

#define ABOUT_OBJECT_INDEX                                      NUM_PRE_ABOUT_OBJECTS

#define ABOUT_GET_PROP                                          AJ_APP_MESSAGE_ID(ABOUT_OBJECT_INDEX, 0, AJ_PROP_GET)
#define ABOUT_SET_PROP                                          AJ_APP_MESSAGE_ID(ABOUT_OBJECT_INDEX, 0, AJ_PROP_SET)

#define ABOUT_VERSION_PROP                                      AJ_APP_PROPERTY_ID(ABOUT_OBJECT_INDEX, 1, 0)
#define ABOUT_GET_ABOUT_DATA                                    AJ_APP_MESSAGE_ID(ABOUT_OBJECT_INDEX, 1, 1)
#define ABOUT_GET_OBJECT_DESCRIPTION                            AJ_APP_MESSAGE_ID(ABOUT_OBJECT_INDEX, 1, 2)
#define ABOUT_ANNOUNCE                                          AJ_APP_MESSAGE_ID(ABOUT_OBJECT_INDEX, 1, 3)

static const char* const AJ_AboutInterface[] = {
    "org.alljoyn.About",
    "@Version>q",
    "?GetAboutData <s >a{sv}",
    "?GetObjectDescription >a(oas)",
    "!Announce >q >q >a(oas) >a{sv}",
    NULL
};

static const uint16_t AJ_AboutVersion = 1;

const AJ_InterfaceDescription AJ_AboutInterfaces[] = {
    AJ_PropertiesIface,
    AJ_AboutInterface,
    NULL
};

static uint16_t appServicePort = 0;
static AJ_Object DefaultAnnounceObjects[] = {
    ABOUT_ANNOUNCEOBJECTS \
    { NULL, NULL }
};
static AJ_Object* appAnnounceObjects = DefaultAnnounceObjects;

AJ_Status AJ_About_Start(uint16_t servicePort, AJ_Object* announceObjects)
{
    AJ_Status status = AJ_OK;

    if (servicePort > 0) {
        appServicePort = servicePort;
    } else {
        AJ_Printf("AJ_About_Start(): Service Port is not set!\n");
        status = AJ_ERR_INVALID;
    }
    if (announceObjects != NULL) {
        appAnnounceObjects = announceObjects;
    }

    return status;
}

/*
 * Handles a property GET request so marshals the property value to return
 */
AJ_Status AJ_About_PropGetHandler(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    if (propId == ABOUT_VERSION_PROP) {
        uint16_t q = AJ_AboutVersion;
        return AJ_MarshalArgs(replyMsg, "q", q);
    } else {
        return AJ_ERR_UNEXPECTED;
    }
}

/*
 * Handles a property SET request so unmarshals the property value provided
 */
AJ_Status AJ_About_PropSetHandler(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    return AJ_ERR_UNEXPECTED;
}

AJ_Status AJ_About_GetAboutData(AJ_Message* msg)
{
    AJ_Status status = AJ_OK;
    AJ_Message reply;
    char* language;
    int8_t langIndex = AJSVC_PROPERTY_STORE_ERROR_LANGUAGE_INDEX;

    AJ_Printf("GetAboutData()\n");

    AJSVC_PropertyStoreCategoryFilter filter;
    memset(&filter, 0, sizeof(AJSVC_PropertyStoreCategoryFilter));
    filter.bit0About = TRUE;

    do {
        CHECK(AJ_UnmarshalArgs(msg, "s", &language));
        if (AJSVC_IsLanguageSupported(msg, &reply, language, &langIndex)) {
            CHECK(AJ_MarshalReplyMsg(msg, &reply));
            CHECK(AJSVC_PropertyStore_ReadAll(&reply, filter, langIndex));
        }
        CHECK(AJ_DeliverMsg(&reply));
    } while (0);

    return status;
}

static const char* StripLeadingSecurityDollar(const char* ifaceName)
{
    return ifaceName + (ifaceName && (*ifaceName == '$'));
}

static AJ_Status AJ_About_GetObjectDescriptionInternal(AJ_Message* msg, uint8_t justParse)
{
    AJ_Status status = AJ_OK;
    AJ_Arg structure;
    AJ_Arg array, array2;
    AJ_Message reply;
    int i = 0, j = 1;
    AJ_Message* amsg = NULL;

    if (justParse) {
        amsg = msg;
    } else {
        amsg = &reply;
    }

    do {
        AJ_Printf("GetObjectDescriptionInternal()\n");
        if (!justParse) {
            CHECK(AJ_MarshalReplyMsg(msg, amsg));
        }

        CHECK(AJ_MarshalContainer(amsg, &array, AJ_ARG_ARRAY));
        AJ_Object current = appAnnounceObjects[i];

        for (; current.path != NULL; current = appAnnounceObjects[++i]) {

            CHECK(AJ_MarshalContainer(amsg, &structure, AJ_ARG_STRUCT));
            CHECK(AJ_MarshalArgs(amsg, "o", current.path));

            CHECK(AJ_MarshalContainer(amsg, &array2, AJ_ARG_ARRAY));

            if ((current.interfaces[0] != NULL) && (current.interfaces[0] != AJ_PropertiesIface)) {
                CHECK(AJ_MarshalArgs(amsg, "s", StripLeadingSecurityDollar(current.interfaces[0][0])));
                AJ_Printf("ObjPath=%s Iface=%s\n", current.path, StripLeadingSecurityDollar(current.interfaces[0][0]));
            }
            for (j = 1; current.interfaces[j]; j++) {
                CHECK(AJ_MarshalArgs(amsg, "s", StripLeadingSecurityDollar(current.interfaces[j][0])));
                AJ_Printf("ObjPath=%s Iface=%s\n", current.path, StripLeadingSecurityDollar(current.interfaces[j][0]));
            }
            CHECK(AJ_MarshalCloseContainer(amsg, &array2));
            CHECK(AJ_MarshalCloseContainer(amsg, &structure));

        }
        CHECK(AJ_MarshalCloseContainer(amsg, &array));
        if (!justParse) {
            CHECK(AJ_DeliverMsg(amsg));
        }
    } while (0);

    return status;
}

AJ_Status AJ_About_GetObjectDescription(AJ_Message* msg)
{
    return AJ_About_GetObjectDescriptionInternal(msg, FALSE);
}

AJ_Status AJ_About_Announce(AJ_BusAttachment* busAttachment)
{
    AJ_Status status = AJ_OK;
    do {
        const char* busUniqueName = AJ_GetUniqueName(busAttachment);
        if (busUniqueName == NULL) {
            AJ_Printf("Failed to GetUniqueName() from a supposedly connected bus\n");
            return AJ_ERR_NULL;
        }
        AJ_Printf("Announce() {sender=%s, port=%d}\n", busUniqueName, appServicePort);
        AJ_Message out;
        CHECK(AJ_MarshalSignal(busAttachment, &out, ABOUT_ANNOUNCE, NULL, 0, ALLJOYN_FLAG_SESSIONLESS, 0));
        uint16_t version = AJ_AboutVersion;
        CHECK(AJ_MarshalArgs(&out, "q", version));
        uint16_t port = appServicePort;
        CHECK(AJ_MarshalArgs(&out, "q", port));
        CHECK(AJ_About_GetObjectDescriptionInternal(&out, TRUE));
        AJSVC_PropertyStoreCategoryFilter filter;
        memset(&filter, 0, sizeof(AJSVC_PropertyStoreCategoryFilter));
        filter.bit2Announce = TRUE;
        CHECK(AJSVC_PropertyStore_ReadAll(&out, filter, AJSVC_PropertyStore_GetCurrentDefaultLanguageIndex()));
        CHECK(AJ_DeliverMsg(&out));
        CHECK(AJ_CloseMsg(&out));
    } while (0);

    return status;
}

static uint8_t bShouldAnnounce = TRUE;

uint8_t AJ_About_IsShouldAnnounce()
{
    return bShouldAnnounce;
}

void AJ_About_SetShouldAnnounce(uint8_t shouldAnnounce)
{
    bShouldAnnounce = shouldAnnounce;
}

AJ_Status AJ_About_ConnectedHandler(AJ_BusAttachment* busAttachment)
{
    return AJ_OK;
}

AJSVC_ServiceStatus AJ_About_MessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status* msgStatus)
{
    AJSVC_ServiceStatus serviceStatus = AJSVC_SERVICE_STATUS_HANDLED;

    switch (msg->msgId) {

/* About Service processing */
    case ABOUT_GET_PROP:
        *msgStatus = AJ_BusPropGet(msg, AJ_About_PropGetHandler, NULL);
        break;

    case ABOUT_SET_PROP:
        *msgStatus = AJ_BusPropSet(msg, AJ_About_PropSetHandler, NULL);
        break;

    case ABOUT_GET_ABOUT_DATA:
        *msgStatus = AJ_About_GetAboutData(msg);
        break;

    case ABOUT_GET_OBJECT_DESCRIPTION:
        *msgStatus = AJ_About_GetObjectDescription(msg);
        break;

    default:
        serviceStatus = AJSVC_SERVICE_STATUS_NOT_HANDLED;
        break;
    }

/* About Icon processing */
    if (serviceStatus == AJSVC_SERVICE_STATUS_NOT_HANDLED) {
        serviceStatus = AJ_AboutIcon_MessageProcessor(bus, msg, msgStatus);
    }

    return serviceStatus;
}

AJ_Status AJ_About_DisconnectHandler(AJ_BusAttachment* busAttachment)
{
    return AJ_OK;
}
