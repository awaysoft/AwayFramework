#include "awaywindow.h"
#include "../global.h"
#include "../ui.h"
#include <gtk/gtk.h>

/* ==============================================
                   Away.Window
   ============================================== */

/* Init && Final */
static void window_Initialize(JSContextRef ctx, JSObjectRef object)
{
}

static void window_Finalize(JSObjectRef object)
{
}

static JSValueRef window_SetLeft(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    if (argumentCount > 0) {
        /* Get Arguments */
        int left = (int)JSValueToNumber(ctx, arguments[0], exception);
        af_window_set_left(left);
    }

    return JSValueMakeUndefined(ctx);
}

static JSValueRef window_SetTop(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    if (argumentCount > 0) {
        /* Get Arguments */
        int top = (int)JSValueToNumber(ctx, arguments[0], exception);
        af_window_set_top(top);
    }

    return JSValueMakeUndefined(ctx);
}

static JSValueRef window_SetPosition(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    if (argumentCount > 1) {
        /* Get Arguments */
        int left = (int)JSValueToNumber(ctx, arguments[0], exception);
        int top = (int)JSValueToNumber(ctx, arguments[1], exception);
        af_window_set_position(left, top);
    }

    return JSValueMakeUndefined(ctx);
}

static JSValueRef window_GetLeft(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    gint x;
    x = af_window_get_left();

    return JSValueMakeNumber(ctx, x);
}

static JSValueRef window_GetTop(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    gint y;
    y = af_window_get_top();

    return JSValueMakeNumber(ctx, y);
}

static JSValueRef window_GetWidth(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    gint y;
    y = af_window_get_width();

    return JSValueMakeNumber(ctx, y);
}

static JSValueRef window_GetHeight(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    gint y;
    y = af_window_get_height();

    return JSValueMakeNumber(ctx, y);
}

static JSValueRef window_GetFullscreen(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    gboolean y;
    y = af_window_get_fullscreen();

    return JSValueMakeBoolean(ctx, y);
}

static JSValueRef window_GetTitle(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    JSStringRef y = NULL;
    y = JSStringCreateWithUTF8CString(af_window_get_title());

    return JSValueMakeString(ctx, y);
}

static JSValueRef window_GetVisible(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    gboolean y;
    y = af_window_get_visible();

    return JSValueMakeBoolean(ctx, y);
}

static JSValueRef window_GetResizable(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    gboolean y;
    y = af_window_get_resizable();

    return JSValueMakeBoolean(ctx, y);
}

static JSValueRef window_GetActive(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    gboolean y;
    y = af_window_get_active();

    return JSValueMakeBoolean(ctx, y);
}

static JSValueRef window_GetBorder(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    gboolean y;
    y = af_window_get_border();

    return JSValueMakeBoolean(ctx, y);
}

static JSValueRef window_SetWidth(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    if (argumentCount > 0) {
        /* Get Arguments */
        int top = (int)JSValueToNumber(ctx, arguments[0], exception);
        af_window_set_top(top);
    }

    return JSValueMakeUndefined(ctx);
}

static JSValueRef window_SetHeight(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    if (argumentCount > 0) {
        /* Get Arguments */
        int top = (int)JSValueToNumber(ctx, arguments[0], exception);
        af_window_set_height(top);
    }

    return JSValueMakeUndefined(ctx);
}

static JSValueRef window_SetFullscreen(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    if (argumentCount > 0) {
        /* Get Arguments */
        gboolean top = JSValueToBoolean(ctx, arguments[0]);
        af_window_set_fullscreen(top);
    }

    return JSValueMakeUndefined(ctx);
}

static JSValueRef window_SetTitle(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    if (argumentCount > 0) {
        /* Get Arguments */
        JSStringRef str = JSValueToStringCopy(ctx, arguments[0], exception);
        size_t size = JSStringGetMaximumUTF8CStringSize(str);
        char* title = g_new(char, size);
        JSStringGetUTF8CString(str, title, size);
        af_window_set_title(title);
        g_free(title);
    }

    return JSValueMakeUndefined(ctx);
}

static JSValueRef window_SetResizable(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    if (argumentCount > 0) {
        /* Get Arguments */
        gboolean top = JSValueToBoolean(ctx, arguments[0]);
        af_window_set_resizable(top);
    }

    return JSValueMakeUndefined(ctx);
}

static JSValueRef window_SetVisible(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    if (argumentCount > 0) {
        /* Get Arguments */
        gboolean top = JSValueToBoolean(ctx, arguments[0]);
        af_window_set_visible(top);
    }

    return JSValueMakeUndefined(ctx);
}

static JSValueRef window_SetBorder(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    if (argumentCount > 0) {
        /* Get Arguments */
        gboolean top = JSValueToBoolean(ctx, arguments[0]);
        af_window_set_border(top);
    }

    return JSValueMakeUndefined(ctx);
}

static JSValueRef window_Maximize(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    return JSValueMakeBoolean(ctx, af_window_maximize());
}

static JSValueRef window_Minimize(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    return JSValueMakeBoolean(ctx, af_window_minimize());
}

static JSValueRef window_Restore(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    return JSValueMakeBoolean(ctx, af_window_restore());
}

static JSValueRef window_GetState(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    return JSValueMakeNumber(ctx, af_window_get_state());
}

static JSValueRef window_SetAbove(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    if (argumentCount > 0) {
        /* Get Arguments */
        gboolean top = JSValueToBoolean(ctx, arguments[0]);
        af_window_set_above(top);
    }

    return JSValueMakeUndefined(ctx);
}

static JSValueRef window_SetBelow(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef *exception)
{
    if (argumentCount > 0) {
        /* Get Arguments */
        gboolean top = JSValueToBoolean(ctx, arguments[0]);
        af_window_set_below(top);
    }

    return JSValueMakeUndefined(ctx);
}

JSClassRef Window_ClassCreate(JSContextRef ctx)
{
    static JSClassRef windowClass = NULL;
    if (windowClass) {
        return windowClass;
    }

    JSStaticFunction windowStaticFunctions[] = {
        { "SetLeft",           window_SetLeft,           kJSPropertyAttributeNone },
        { "SetTop",            window_SetTop,            kJSPropertyAttributeNone },
        { "SetPosition",       window_SetPosition,       kJSPropertyAttributeNone },
        { "GetLeft",           window_GetLeft,           kJSPropertyAttributeNone },
        { "GetTop",            window_GetTop,            kJSPropertyAttributeNone },
        { "GetWidth",          window_GetWidth,          kJSPropertyAttributeNone },
        { "GetHeight",         window_GetHeight,         kJSPropertyAttributeNone },
        { "GetFullscreen",     window_GetFullscreen,     kJSPropertyAttributeNone },
        { "GetTitle",          window_GetTitle,          kJSPropertyAttributeNone },
        { "GetVisible",        window_GetVisible,        kJSPropertyAttributeNone },
        { "GetResizable",      window_GetResizable,      kJSPropertyAttributeNone },
        { "GetActive",         window_GetActive,         kJSPropertyAttributeNone },
        { "GetBorder",         window_GetBorder,         kJSPropertyAttributeNone },
        { "SetWidth",          window_SetWidth,          kJSPropertyAttributeNone },
        { "SetHeight",         window_SetHeight,         kJSPropertyAttributeNone },
        { "SetFullscreen",     window_SetFullscreen,     kJSPropertyAttributeNone },
        { "SetTitle",          window_SetTitle,          kJSPropertyAttributeNone },
        { "SetResizable",      window_SetResizable,      kJSPropertyAttributeNone },
        { "SetVisible",        window_SetVisible,        kJSPropertyAttributeNone },
        { "SetBorder",         window_SetBorder,         kJSPropertyAttributeNone },
        { "Maximize",          window_Maximize,          kJSPropertyAttributeNone },
        { "Minimize",          window_Minimize,          kJSPropertyAttributeNone },
        { "Restore",           window_Restore,           kJSPropertyAttributeNone },
        { "GetState",          window_GetState,          kJSPropertyAttributeNone },
        { "SetAbove",          window_SetAbove,          kJSPropertyAttributeNone },
        { "SetBelow",          window_SetBelow,          kJSPropertyAttributeNone },
        { NULL, 0, 0 },
    };

    JSStaticValue windowStaticValues[] = {
        { NULL, 0, 0, 0},
    };

    JSClassDefinition classdef = kJSClassDefinitionEmpty;
    classdef.className         = "AWindow";
    classdef.initialize        = window_Initialize;
    classdef.finalize          = window_Finalize;
    classdef.staticValues      = windowStaticValues;
    classdef.staticFunctions   = windowStaticFunctions;

    return windowClass = JSClassCreate(&classdef);
}
