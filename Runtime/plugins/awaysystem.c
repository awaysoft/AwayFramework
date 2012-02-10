#include "awaywindow.h"
#include "awaymouse.h"

/* ==============================================
                    Away.System
   ============================================== */

/* Init && Final */
static void system_Initialize(JSContextRef context, JSObjectRef object)
{
    /* Add Away.Window */
    JSStringRef window_name = JSStringCreateWithUTF8CString("Window");
    JSObjectRef window_obj = JSObjectMake(context, Window_ClassCreate(context), NULL);
    JSObjectSetProperty(context, object, window_name, window_obj ,kJSPropertyAttributeNone, NULL);

    /* Add Away.Mouse */
    JSStringRef mouse_name = JSStringCreateWithUTF8CString("Mouse");
    JSObjectRef mouse_obj = JSObjectMake(context, Mouse_ClassCreate(context), NULL);
    JSObjectSetProperty(context, object, mouse_name, mouse_obj ,kJSPropertyAttributeNone, NULL);
}

static void system_Finalize(JSObjectRef object)
{
}

JSClassRef System_ClassCreate(JSContextRef ctx)
{
    static JSClassRef systemClass = NULL;
    if (systemClass) {
        return systemClass;
    }

    JSStaticFunction systemStaticFunctions[] = {
        { NULL, 0, 0 },
    };

    JSStaticValue systemStaticValues[] = {
        { NULL, 0, 0, 0},
    };

    JSClassDefinition classdef = kJSClassDefinitionEmpty;
    classdef.className         = "ASystem";
    classdef.initialize        = system_Initialize;
    classdef.finalize          = system_Finalize;
    classdef.staticValues      = systemStaticValues;
    classdef.staticFunctions   = systemStaticFunctions;

    return systemClass = JSClassCreate(&classdef);
}

