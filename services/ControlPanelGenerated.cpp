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

/**
 * Per-module definition of the current module for debug logging.  Must be defined
 * prior to first inclusion of aj_debug.h.
 * The corresponding flag dbgAJSVCAPP is defined in the containing sample app.
 */
#define AJ_MODULE AJSVCAPP
#include <aj_debug.h>


#include <alljoyn.h>
#include <ConstraintList.h>
#include <ControlPanelService.h>
#include "ControlPanelProvided.h"

#ifndef NDEBUG
extern AJ_EXPORT uint8_t dbgAJSVCAPP;
#endif

const char TriRootContainerObjectPath[] = "/ControlPanel/Tri/rootContainer";
const char enTriRootContainerObjectPath[] = "/ControlPanel/Tri/rootContainer/en";
const char de_ATTriRootContainerObjectPath[] = "/ControlPanel/Tri/rootContainer/de_AT";
const char enTriHeatPropertyObjectPath[] = "/ControlPanel/Tri/rootContainer/en/heatProperty";
const char de_ATTriHeatPropertyObjectPath[] = "/ControlPanel/Tri/rootContainer/de_AT/heatProperty";
const char enTriLightActionObjectPath[] = "/ControlPanel/Tri/rootContainer/en/lightAction";
const char de_ATTriLightActionObjectPath[] = "/ControlPanel/Tri/rootContainer/de_AT/lightAction";
const char enTriLightConfirmObjectPath[] = "/ControlPanel/Tri/rootContainer/en/lightAction/LightConfirm";
const char de_ATTriLightConfirmObjectPath[] = "/ControlPanel/Tri/rootContainer/de_AT/lightAction/LightConfirm";
const char TriAreYouSureObjectPath[] = "/ControlPanel/Tri/areYouSure";
const char enTriAreYouSureObjectPath[] = "/ControlPanel/Tri/areYouSure/en";
const char de_ATTriAreYouSureObjectPath[] = "/ControlPanel/Tri/areYouSure/de_AT";

ContainerWidget TriRootContainer;
PropertyWidget TriHeatProperty;
ActionWidget TriLightAction;
DialogWidget TriLightConfirm;
DialogWidget TriAreYouSure;

/*
 * Static variables used to fill widgets
 */
static const char* const TriRootContainerLabel[] = { "Control Panel", "Container Etikett" };
static const uint16_t TriRootContainerHints[] = { LAYOUT_HINT_VERTICAL_LINEAR, LAYOUT_HINT_HORIZONTAL_LINEAR };
static const char* const TriHeatPropertySignature = "q";
static const char* const TriHeatPropertyLabel[] = { "Light Bar", "Ofentemperatur" };
static const uint16_t TriHeatPropertyHints[] = { PROPERTY_WIDGET_HINT_SLIDER };
static const char* const TriHeatPropertyUnitMeasure[] = { "Grid", "Grad" };
static const uint16_t TriHeatPropertyConstraintRangeMin = 0;
static const uint16_t TriHeatPropertyConstraintRangeMax = 10;
static const uint16_t TriHeatPropertyConstraintRangeInc = 1;
static const char* const TriLightActionLabel[] = { "Turn on oven light", "Ofenlicht anschalten" };
static const uint16_t TriLightActionHints[] = { ACTION_WIDGET_HINT_ACTIONBUTTON };
static const char* const TriLightConfirmMessage[] = { "Are you sure you want to turn on the light", "Are you sure you want to turn on the light" };
static const char* const TriLightConfirmLabel[] = { "Are you sure?", "Sind sie sicher?" };
static const uint16_t TriLightConfirmHints[] = { DIALOG_HINT_ALERTDIALOG };
static const char* const TriLightConfirmLabelaction1[] = { "Yes", "Ja" };
static const char* const TriLightConfirmLabelaction2[] = { "No", "Nein" };
static const char* const TriLightConfirmLabelaction3[] = { "Cancel", "Abrechen" };
static const char* const TriAreYouSureMessage[] = { "Are you sure?", "Sind sie sicher?" };
static const char* const TriAreYouSureLabel[] = { "Are you sure?", "Sind sie sicher?" };
static const char* const TriAreYouSureLabelaction1[] = { "Yes", "Ja" };


void WidgetsInit()
{
    initializeContainerWidget(&TriRootContainer);
    TriRootContainer.base.numLanguages = 2;
    setBaseEnabled(&TriRootContainer.base, TRUE);

    TriRootContainer.base.optParams.bgColor = 0x200;
    TriRootContainer.base.optParams.label = TriRootContainerLabel;
    TriRootContainer.base.optParams.hints = TriRootContainerHints;
    TriRootContainer.base.optParams.numHints = 2;

    initializePropertyWidget(&TriHeatProperty);
    TriHeatProperty.base.numLanguages = 2;
    setBaseEnabled(&TriHeatProperty.base, TRUE);
    setBaseWritable(&TriHeatProperty.base, TRUE);

    TriHeatProperty.signature = TriHeatPropertySignature;
    TriHeatProperty.propertyType = SINGLE_VALUE_PROPERTY;
    TriHeatProperty.getValue = &getuint16Var;

    TriHeatProperty.base.optParams.bgColor = 0x500;
    TriHeatProperty.base.optParams.label = TriHeatPropertyLabel;
    TriHeatProperty.base.optParams.hints = TriHeatPropertyHints;
    TriHeatProperty.base.optParams.numHints = 1;
    TriHeatProperty.optParams.unitOfMeasure = TriHeatPropertyUnitMeasure;

    TriHeatProperty.optParams.constraintRangeDefined = TRUE;
    TriHeatProperty.optParams.constraintRange.minValue = &TriHeatPropertyConstraintRangeMin;
    TriHeatProperty.optParams.constraintRange.maxValue = &TriHeatPropertyConstraintRangeMax;
    TriHeatProperty.optParams.constraintRange.increment = &TriHeatPropertyConstraintRangeInc;

    initializeActionWidget(&TriLightAction);
    TriLightAction.base.numLanguages = 2;
    setBaseEnabled(&TriLightAction.base, TRUE);

    TriLightAction.base.optParams.bgColor = 0x400;
    TriLightAction.base.optParams.label = TriLightActionLabel;
    TriLightAction.base.optParams.hints = TriLightActionHints;
    TriLightAction.base.optParams.numHints = 1;
    initializeDialogWidget(&TriLightConfirm);
    TriLightConfirm.base.numLanguages = 2;
    setBaseEnabled(&TriLightConfirm.base, TRUE);
    TriLightConfirm.message = TriLightConfirmMessage;
    TriLightConfirm.numActions = 3;

    TriLightConfirm.base.optParams.bgColor = 0x789;
    TriLightConfirm.base.optParams.label = TriLightConfirmLabel;
    TriLightConfirm.base.optParams.hints = TriLightConfirmHints;
    TriLightConfirm.base.optParams.numHints = 1;
    TriLightConfirm.optParams.labelAction1 = TriLightConfirmLabelaction1;
    TriLightConfirm.optParams.labelAction2 = TriLightConfirmLabelaction2;
    TriLightConfirm.optParams.labelAction3 = TriLightConfirmLabelaction3;
    initializeDialogWidget(&TriAreYouSure);
    TriAreYouSure.base.numLanguages = 2;
    setBaseEnabled(&TriAreYouSure.base, TRUE);
    TriAreYouSure.message = TriAreYouSureMessage;
    TriAreYouSure.numActions = 1;

    TriAreYouSure.base.optParams.bgColor = 0x789;
    TriAreYouSure.base.optParams.label = TriAreYouSureLabel;
    TriAreYouSure.optParams.labelAction1 = TriAreYouSureLabelaction1;

    return;
}

void* IdentifyMsgOrPropId(uint32_t identifier, uint16_t* widgetType, uint16_t* propType, uint16_t* language)
{
    switch (identifier) {
    case EN_TRI_ROOTCONTAINER_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *language = TRI_MYLANGUAGES_EN;
        return &TriRootContainer;

    case EN_TRI_ROOTCONTAINER_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_VERSION;
        *language = TRI_MYLANGUAGES_EN;
        return &TriRootContainer;

    case EN_TRI_ROOTCONTAINER_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_STATES;
        *language = TRI_MYLANGUAGES_EN;
        return &TriRootContainer;

    case EN_TRI_ROOTCONTAINER_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = TRI_MYLANGUAGES_EN;
        return &TriRootContainer;

    case DE_AT_TRI_ROOTCONTAINER_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriRootContainer;

    case DE_AT_TRI_ROOTCONTAINER_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_VERSION;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriRootContainer;

    case DE_AT_TRI_ROOTCONTAINER_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_STATES;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriRootContainer;

    case DE_AT_TRI_ROOTCONTAINER_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_CONTAINER;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriRootContainer;

    case EN_TRI_HEATPROPERTY_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = TRI_MYLANGUAGES_EN;
        return &TriHeatProperty;

    case EN_TRI_HEATPROPERTY_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = TRI_MYLANGUAGES_EN;
        return &TriHeatProperty;

    case EN_TRI_HEATPROPERTY_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = TRI_MYLANGUAGES_EN;
        return &TriHeatProperty;

    case EN_TRI_HEATPROPERTY_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = TRI_MYLANGUAGES_EN;
        return &TriHeatProperty;

    case EN_TRI_HEATPROPERTY_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = TRI_MYLANGUAGES_EN;
        return &TriHeatProperty;

    case DE_AT_TRI_HEATPROPERTY_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriHeatProperty;

    case DE_AT_TRI_HEATPROPERTY_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VERSION;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriHeatProperty;

    case DE_AT_TRI_HEATPROPERTY_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_STATES;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriHeatProperty;

    case DE_AT_TRI_HEATPROPERTY_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriHeatProperty;

    case DE_AT_TRI_HEATPROPERTY_VALUE_PROPERTY:
        *widgetType = WIDGET_TYPE_PROPERTY;
        *propType = PROPERTY_TYPE_VALUE;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriHeatProperty;

    case EN_TRI_LIGHTACTION_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_ACTION;
        *language = TRI_MYLANGUAGES_EN;
        return &TriLightAction;

    case EN_TRI_LIGHTACTION_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_VERSION;
        *language = TRI_MYLANGUAGES_EN;
        return &TriLightAction;

    case EN_TRI_LIGHTACTION_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_STATES;
        *language = TRI_MYLANGUAGES_EN;
        return &TriLightAction;

    case EN_TRI_LIGHTACTION_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = TRI_MYLANGUAGES_EN;
        return &TriLightAction;

    case DE_AT_TRI_LIGHTACTION_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_ACTION;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriLightAction;

    case DE_AT_TRI_LIGHTACTION_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_VERSION;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriLightAction;

    case DE_AT_TRI_LIGHTACTION_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_STATES;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriLightAction;

    case DE_AT_TRI_LIGHTACTION_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_ACTION;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriLightAction;

    case EN_TRI_LIGHTCONFIRM_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_DIALOG;
        *language = TRI_MYLANGUAGES_EN;
        return &TriLightConfirm;

    case EN_TRI_LIGHTCONFIRM_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_VERSION;
        *language = TRI_MYLANGUAGES_EN;
        return &TriLightConfirm;

    case EN_TRI_LIGHTCONFIRM_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_STATES;
        *language = TRI_MYLANGUAGES_EN;
        return &TriLightConfirm;

    case EN_TRI_LIGHTCONFIRM_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = TRI_MYLANGUAGES_EN;
        return &TriLightConfirm;

    case DE_AT_TRI_LIGHTCONFIRM_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_DIALOG;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriLightConfirm;

    case DE_AT_TRI_LIGHTCONFIRM_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_VERSION;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriLightConfirm;

    case DE_AT_TRI_LIGHTCONFIRM_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_STATES;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriLightConfirm;

    case DE_AT_TRI_LIGHTCONFIRM_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriLightConfirm;

    case EN_TRI_AREYOUSURE_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_DIALOG;
        *language = TRI_MYLANGUAGES_EN;
        return &TriAreYouSure;

    case EN_TRI_AREYOUSURE_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_VERSION;
        *language = TRI_MYLANGUAGES_EN;
        return &TriAreYouSure;

    case EN_TRI_AREYOUSURE_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_STATES;
        *language = TRI_MYLANGUAGES_EN;
        return &TriAreYouSure;

    case EN_TRI_AREYOUSURE_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = TRI_MYLANGUAGES_EN;
        return &TriAreYouSure;

    case DE_AT_TRI_AREYOUSURE_GET_ALL_VALUES:
        *widgetType = WIDGET_TYPE_DIALOG;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriAreYouSure;

    case DE_AT_TRI_AREYOUSURE_VERSION_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_VERSION;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriAreYouSure;

    case DE_AT_TRI_AREYOUSURE_STATES_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_STATES;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriAreYouSure;

    case DE_AT_TRI_AREYOUSURE_OPTPARAMS_PROPERTY:
        *widgetType = WIDGET_TYPE_DIALOG;
        *propType = PROPERTY_TYPE_OPTPARAMS;
        *language = TRI_MYLANGUAGES_DE_AT;
        return &TriAreYouSure;

    default:
        return FALSE;
    }
}

void* IdentifyMsgOrPropIdForSignal(uint32_t identifier, uint8_t* isProperty)
{
    switch (identifier) {
    case EN_TRI_ROOTCONTAINER_SIGNAL_PROPERTIES_CHANGED:
        return &TriRootContainer;

    case DE_AT_TRI_ROOTCONTAINER_SIGNAL_PROPERTIES_CHANGED:
        return &TriRootContainer;

    case EN_TRI_HEATPROPERTY_SIGNAL_PROPERTIES_CHANGED:
        return &TriHeatProperty;

    case EN_TRI_HEATPROPERTY_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &TriHeatProperty;

    case DE_AT_TRI_HEATPROPERTY_SIGNAL_PROPERTIES_CHANGED:
        return &TriHeatProperty;

    case DE_AT_TRI_HEATPROPERTY_SIGNAL_VALUE_CHANGED:
        *isProperty = TRUE;
        return &TriHeatProperty;

    case EN_TRI_LIGHTACTION_SIGNAL_PROPERTIES_CHANGED:
        return &TriLightAction;

    case DE_AT_TRI_LIGHTACTION_SIGNAL_PROPERTIES_CHANGED:
        return &TriLightAction;

    case EN_TRI_LIGHTCONFIRM_SIGNAL_PROPERTIES_CHANGED:
        return &TriLightConfirm;

    case DE_AT_TRI_LIGHTCONFIRM_SIGNAL_PROPERTIES_CHANGED:
        return &TriLightConfirm;

    case EN_TRI_AREYOUSURE_SIGNAL_PROPERTIES_CHANGED:
        return &TriAreYouSure;

    case DE_AT_TRI_AREYOUSURE_SIGNAL_PROPERTIES_CHANGED:
        return &TriAreYouSure;

    default:
        return FALSE;
    }
}

uint8_t IdentifyRootMsgOrPropId(uint32_t identifier)
{
    switch (identifier) {
    case TRI_ROOT_CONTROLPANEL_ROOTCONTAINER_VERSION_PROPERTY:
    case TRI_ROOT_CONTROLPANEL_ROOTCONTAINER_GET_ALL_VALUES:
        return TRUE;

    case TRI_NOTIFICATION_ACTION_AREYOUSURE_VERSION_PROPERTY:
    case TRI_NOTIFICATION_ACTION_AREYOUSURE_GET_ALL_VALUES:
    case TRI_NOTIFICATION_ACTION_AREYOUSURE_SIGNAL_DISMISS:
        return TRUE;

    default:
        return FALSE;
    }
}

AJ_Status SetValueProperty(AJ_Message* replyMsg, uint32_t propId, void* context)
{
    AJ_Status status = AJ_ERR_UNEXPECTED;

    const char* variantSig;
    AJ_UnmarshalVariant(replyMsg, &variantSig);

    switch (propId) {
    case EN_TRI_HEATPROPERTY_VALUE_PROPERTY:
    case DE_AT_TRI_HEATPROPERTY_VALUE_PROPERTY:
        {
            uint16_t newValue;
            status = unmarshalPropertyValue(&TriHeatProperty, replyMsg, &newValue, ((SetValueContext*)context)->sender);
            if (status != AJ_OK) {
                return status;
            }
            setuint16Var(newValue);
            ((SetValueContext*)context)->numSignals = 2;
            ((SetValueContext*)context)->signals[0] = EN_TRI_HEATPROPERTY_SIGNAL_VALUE_CHANGED;
            ((SetValueContext*)context)->signals[1] = DE_AT_TRI_HEATPROPERTY_SIGNAL_VALUE_CHANGED;
        }
        break;


    }

    return status;
}

AJ_Status ExecuteAction(AJ_Message* msg, uint32_t msgId, ExecuteActionContext* context)
{
    AJ_Message reply;
    AJ_MarshalReplyMsg(msg, &reply);

    switch (msgId) {
    case EN_TRI_LIGHTCONFIRM_EXEC_ACTION1:
    case DE_AT_TRI_LIGHTCONFIRM_EXEC_ACTION1:
        {
            startOven();
        }
        break;

    case EN_TRI_LIGHTCONFIRM_EXEC_ACTION2:
    case DE_AT_TRI_LIGHTCONFIRM_EXEC_ACTION2:
        {
            stopOven();
        }
        break;

    case EN_TRI_LIGHTCONFIRM_EXEC_ACTION3:
    case DE_AT_TRI_LIGHTCONFIRM_EXEC_ACTION3:
        {
            AJ_InfoPrintf(("Execute Action3 was called\n"));
        }
        break;

    case EN_TRI_AREYOUSURE_EXEC_ACTION1:
    case DE_AT_TRI_AREYOUSURE_EXEC_ACTION1:
        {
            AJ_InfoPrintf(("Execute Action1 was called\n"));
	    addDismissSignal(context, TRI_NOTIFICATION_ACTION_AREYOUSURE_SIGNAL_DISMISS);
        }
        break;

    case EN_TRI_AREYOUSURE_EXEC_ACTION2:
    case DE_AT_TRI_AREYOUSURE_EXEC_ACTION2:
        {
            AJ_MarshalErrorMsg(msg, &reply, AJ_ErrServiceUnknown);
        }
        break;

    case EN_TRI_AREYOUSURE_EXEC_ACTION3:
    case DE_AT_TRI_AREYOUSURE_EXEC_ACTION3:
        {
            AJ_MarshalErrorMsg(msg, &reply, AJ_ErrServiceUnknown);
        }
        break;


    }

    return AJ_DeliverMsg(&reply);
}

void TestsInit(CPSTest* testsToRun)
{

}

AJSVC_ServiceStatus GeneratedMessageProcessor(AJ_BusAttachment* bus, AJ_Message* msg, AJ_Status* msgStatus)
{
    AJSVC_ServiceStatus AJSVC_ServiceStatus = AJSVC_SERVICE_STATUS_HANDLED;

    switch (msg->msgId) {
        GET_WIDGET_VALUE_CASES
        *msgStatus = AJ_BusPropGet(msg, AJCPS_GetWidgetProperty, NULL);
        break;

        GET_WIDGET_ALL_VALUE_CASES
        *msgStatus = AJCPS_GetAllWidgetProperties(msg, msg->msgId);
        break;

        GET_ROOT_VALUE_CASES
        *msgStatus = AJ_BusPropGet(msg, AJCPS_GetRootProperty, NULL);
        break;

        GET_ROOT_ALL_VALUE_CASES
        *msgStatus = AJCPS_GetAllRootProperties(msg, msg->msgId);
        break;

        SET_VALUE_CASES
        {
            SetValueContext context;
            context.sender = msg->sender;
            context.numSignals = 0;
            *msgStatus = AJ_BusPropSet(msg, SetValueProperty, &context);
            if (*msgStatus == AJ_OK && context.numSignals != 0) {
                uint16_t indx;
                for (indx = 0; indx < context.numSignals; indx++) {
                    *msgStatus = AJCPS_SendPropertyChangedSignal(bus, context.signals[indx], AJCPS_GetCurrentSessionId());
                }
            }
        }
        break;

        ACTION_CASES
        {
            ExecuteActionContext context;
            context.numSignals = 0;
            *msgStatus = ExecuteAction(msg, msg->msgId, &context);
            if (*msgStatus == AJ_OK && context.numSignals != 0) {
                uint16_t indx;
                for (indx = 0; indx < context.numSignals; indx++) {
                    if (context.signals[indx].signalType == SIGNAL_TYPE_DATA_CHANGED) {
                        *msgStatus = AJCPS_SendPropertyChangedSignal(bus, context.signals[indx].signalId, AJCPS_GetCurrentSessionId());
                    } else if (context.signals[indx].signalType == SIGNAL_TYPE_DISMISS) {
                        *msgStatus = AJCPS_SendDismissSignal(bus, context.signals[indx].signalId, AJCPS_GetCurrentSessionId());
                    }
                }
            }
        }
        break;

        GET_URL_CASES
        *msgStatus = AJCPS_SendRootUrl(msg, msg->msgId);
        break;

    case AJ_SIGNAL_SESSION_LOST:
        break;

    default:
        AJSVC_ServiceStatus = AJSVC_SERVICE_STATUS_NOT_HANDLED;
        break;
    }

    return AJSVC_ServiceStatus;
}
