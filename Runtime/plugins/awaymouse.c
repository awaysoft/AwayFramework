#include "awaymouse.h"
#include "../global.h"
#include <gdk/gdk.h>

/* ==============================================
               Away.System.Mouse
   ============================================== */

/* Init && Final */
static void window_Initialize(JSContextRef ctx, JSObjectRef object)
{
}

static void window_Finalize(JSObjectRef object)
{
}

static JSValueRef window_GetLeft(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    gint x, y;
    GdkDisplay * display = gdk_display_get_default();
    /* Get Position */
    gdk_display_get_pointer(display, NULL, &x, &y, NULL);

    return JSValueMakeNumber(ctx, x);
}

static JSValueRef window_GetTop(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    gint x, y;
    GdkDisplay * display = gdk_display_get_default();
    /* Get Position */
    gdk_display_get_pointer(display, NULL, &x, &y, NULL);

    return JSValueMakeNumber(ctx, y);
}

JSClassRef Mouse_ClassCreate(JSContextRef ctx)
{
    static JSClassRef windowClass = NULL;
    if (windowClass) {
        return windowClass;
    }

    JSStaticFunction windowStaticFunctions[] = {
        { "GetX",              window_GetLeft,           kJSPropertyAttributeNone },
        { "GetY",              window_GetTop,            kJSPropertyAttributeNone },
        { NULL, 0, 0 },
    };

    JSStaticValue windowStaticValues[] = {
        { NULL, 0, 0, 0},
    };

    JSClassDefinition classdef = kJSClassDefinitionEmpty;
    classdef.className         = "AMouse";
    classdef.initialize        = window_Initialize;
    classdef.finalize          = window_Finalize;
    classdef.staticValues      = windowStaticValues;
    classdef.staticFunctions   = windowStaticFunctions;

    return windowClass = JSClassCreate(&classdef);
}
