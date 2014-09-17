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
#include "AboutSample.h"

AJ_Status About_Init(AJ_Object* announceObjects, const char* aboutIconMimetype, const uint8_t* aboutIconContent, const size_t aboutIconContentSize, const char* aboutIconUrl)
{
    AJ_Status status = AJ_About_Start(AJ_ABOUT_SERVICE_PORT, announceObjects);
    if (status == AJ_OK) {
        status = AJ_AboutIcon_Start(aboutIconMimetype, aboutIconContent, aboutIconContentSize, aboutIconUrl);
    }
    return status;
}

void About_DoWork(AJ_BusAttachment* busAttachment)
{
}

AJ_Status About_Finish()
{
    return AJ_OK;
}
