#include "plugins.h"
#include "plugins/awayconst.h"
#include "plugins/awaysystem.h"

#include <JavaScriptCore/JavaScript.h>

struct _plugin {
    char *serial;
    char *title;
    char *version;
    char *versionshow;
    char *frameversion;
    char *index;
    char *icon;
    char *platform;
    char *os;
    char *plugins;
    char *addons;
    char *location;
};

static JSObjectRef away_object = NULL;
static JSObjectRef const_object = NULL;
static JSObjectRef system_object = NULL;
static JSObjectRef plugins_object = NULL;

int load_plugins()
{
    return 0;
}


/* ===============Java Script Core====================== */
static JSClassRef Plugins_ClassCreate(JSContextRef ctx);

static void away_Initialize(JSContextRef context, JSObjectRef object)
{
    /* Add Away.Const */
    JSStringRef const_name = JSStringCreateWithUTF8CString("Const");
    const_object = JSObjectMake(context, Const_ClassCreate(context), NULL);
    JSObjectSetProperty(context, object, const_name, const_object ,kJSPropertyAttributeNone, NULL);

    /* Add Away.System */
    JSStringRef system_name = JSStringCreateWithUTF8CString("System");
    system_object = JSObjectMake(context, System_ClassCreate(context), NULL);
    JSObjectSetProperty(context, object, system_name, system_object ,kJSPropertyAttributeNone, NULL);

    /* Add Away.System */
    JSStringRef plugins_name = JSStringCreateWithUTF8CString("Plugins");
    plugins_object = JSObjectMake(context, Plugins_ClassCreate(context), NULL);
    JSObjectSetProperty(context, object, plugins_name, plugins_object ,kJSPropertyAttributeNone, NULL);
}

static void away_Finalize(JSObjectRef object)
{
}

/* Create Away Object */
static JSClassRef Away_ClassCreate(JSContextRef ctx)
{
    static JSClassRef awayClass = NULL;
    if (awayClass) {
        return awayClass;
    }

    JSStaticFunction awayStaticFunctions[] = {
        { NULL, 0, 0 },
    };

    JSStaticValue awayStaticValues[] = {
        { NULL, 0, 0, 0},
    };

    JSClassDefinition classdef = kJSClassDefinitionEmpty;
    classdef.className         = "Away";
    classdef.initialize        = away_Initialize;
    classdef.finalize          = away_Finalize;
    classdef.staticValues      = awayStaticValues;
    classdef.staticFunctions   = awayStaticFunctions;

    return awayClass = JSClassCreate(&classdef);
}


/* ==============================================
                    Away.Plugins
   ============================================== */

/* Init && Final */
static void plugins_Initialize(JSContextRef context, JSObjectRef object)
{
}

static void plugins_Finalize(JSObjectRef object)
{
}

static JSClassRef Plugins_ClassCreate(JSContextRef ctx)
{
    static JSClassRef systemClass = NULL;
    if (systemClass) {
        return systemClass;
    }

    JSStaticFunction pluginsStaticFunctions[] = {
        { NULL, 0, 0 },
    };

    JSStaticValue pluginsStaticValues[] = {
        { NULL, 0, 0, 0},
    };

    JSClassDefinition classdef = kJSClassDefinitionEmpty;
    classdef.className         = "APlugins";
    classdef.initialize        = plugins_Initialize;
    classdef.finalize          = plugins_Finalize;
    classdef.staticValues      = pluginsStaticValues;
    classdef.staticFunctions   = pluginsStaticFunctions;

    return systemClass = JSClassCreate(&classdef);
}



void js_callback(WebKitWebView  *web_view,
                 WebKitWebFrame *frame,
                 gpointer        context,
                 gpointer        window_object,
                 gpointer        user_data)
{
    JSStringRef away_name = JSStringCreateWithUTF8CString("Away");
// Make the javascript object
    away_object = JSObjectMake(context, Away_ClassCreate(context), NULL);
// Set the property
    JSObjectSetProperty(context, JSContextGetGlobalObject(context), away_name, away_object,kJSPropertyAttributeNone, NULL);
}
