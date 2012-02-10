#include "awayconst.h"
#include "../global.h"
#include <gdk/gdk.h>
#include <string.h>
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#endif

/* ==============================================
                   Away.Const
   ============================================== */
static char *const_getSystemVersion();
static JSClassRef Screen_ClassCreate(JSContextRef ctx);

/* Version */
static JSValueRef const_GetVersion(
    JSContextRef ctx,
    JSObjectRef  object,
    JSStringRef  propertyName,
    JSValueRef  *exception)
{
    static int version = 0;
    if (version == 0) {
        version = VERSION;
    }
    return JSValueMakeNumber(ctx, version);
}

/* Platform */
static JSValueRef const_GetPlatform(
    JSContextRef ctx,
    JSObjectRef  object,
    JSStringRef  propertyName,
    JSValueRef  *exception)
{
    static int version = 0;
    if (version == 0) {
        version = PLATFORM;
    }
    return JSValueMakeNumber(ctx, version);
}

/* Opertion System */
static JSValueRef const_GetOS(
    JSContextRef ctx,
    JSObjectRef  object,
    JSStringRef  propertyName,
    JSValueRef  *exception)
{
static int version = 0;
    if (version == 0) {
        version = OS;
    }
    return JSValueMakeNumber(ctx, version);
}

/* Opertion System Detail */
static JSValueRef const_GetOSDetail(
    JSContextRef ctx,
    JSObjectRef  object,
    JSStringRef  propertyName,
    JSValueRef  *exception)
{
    static JSStringRef version = NULL;
    if (version == NULL) {
        version = JSStringCreateWithUTF8CString(const_getSystemVersion());
    }
    return JSValueMakeString(ctx, version);
}

/* Version Show */
static JSValueRef const_GetVersionShow(
    JSContextRef ctx,
    JSObjectRef  object,
    JSStringRef  propertyName,
    JSValueRef  *exception)
{
    static JSStringRef version = NULL;
    if (version == NULL) {
        version = JSStringCreateWithUTF8CString(VERSION_SHOW);
    }
    return JSValueMakeString(ctx, version);
}

/* Init && Final */
static void const_Initialize(JSContextRef ctx, JSObjectRef object)
{
    JSStringRef screen_name = JSStringCreateWithUTF8CString("Screen");
    JSObjectRef screen_obj = JSObjectMake(ctx, Screen_ClassCreate(ctx), NULL);
    JSObjectSetProperty(ctx, object, screen_name, screen_obj ,kJSPropertyAttributeNone, NULL);
}

static void const_Finalize(JSObjectRef object)
{
}

/* Create Const Object */
JSClassRef Const_ClassCreate(JSContextRef ctx)
{
    static JSClassRef constClass = NULL;
    if (constClass) {
        return constClass;
    }

    JSStaticFunction constStaticFunctions[] = {
        { NULL, 0, 0 },
    };

    JSStaticValue constStaticValues[] = {
        { "Platform",   const_GetPlatform,  NULL,  kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { "OS",   const_GetOS,  NULL,  kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { "OSDetail",   const_GetOSDetail,  NULL,  kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { "Version",   const_GetVersion,  NULL,  kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { "VersionShow",   const_GetVersionShow,  NULL,  kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { NULL, 0, 0, 0},
    };

    JSClassDefinition classdef = kJSClassDefinitionEmpty;
    classdef.className         = "AConst";
    classdef.initialize        = const_Initialize;
    classdef.finalize          = const_Finalize;
    classdef.staticValues      = constStaticValues;
    classdef.staticFunctions   = constStaticFunctions;

    return constClass = JSClassCreate(&classdef);
}

#ifdef WIN32

static char * _T(char *s){return s;}

static char *const_getSystemVersion()
{
    SYSTEM_INFO info;                                   //用SYSTEM_INFO结构判断64位AMD处理器
    GetSystemInfo(&info);                               //调用GetSystemInfo函数填充结构
    OSVERSIONINFOEX os;
    os.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);  /*在调用函数前必须用sizeof(OSVERSIONINFOEXA)填充dwOSVersionInfoSize结构成员*/
    if(GetVersionEx((OSVERSIONINFO *)&os))                  /*调用GetVersionEx函数OSVERSIONINFOEX结构必须将指针类型强制转换*/
    {
        char * vname;

        //下面根据版本信息判断操作系统名称

        switch(os.dwMajorVersion){                        //判断主版本号
            case 4:
                switch(os.dwMinorVersion){                //判断次版本号
                    case 0:
                        if(os.dwPlatformId==VER_PLATFORM_WIN32_NT)
                            vname=_T("Microsoft Windows NT 4.0");                //1996年7月发布
                        else if(os.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
                            vname=_T("Microsoft Windows 95");
                        break;
                    case 10:
                        vname=_T("Microsoft Windows 98");
                        break;
                    case 90:
                        vname=_T("Microsoft Windows Me");
                        break;
                }
                break;
            case 5:
                switch(os.dwMinorVersion){               //再比较dwMinorVersion的值
                    case 0:
                        vname=_T("Microsoft Windows 2000");                    //1999年12月发布
                        break;
                    case 1:
                        vname=_T("Microsoft Windows XP");                    //2001年8月发布
                        break;
                    case 2:
                        if(os.wProductType==VER_NT_WORKSTATION &&
                           info.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)

                            vname=_T("Microsoft Windows XP Professional x64 Edition");
                        else if(GetSystemMetrics(SM_SERVERR2)==0)
                            vname=_T("Microsoft Windows Server 2003");        //2003年3月发布
                        else if(GetSystemMetrics(SM_SERVERR2)!=0)
                            vname=_T("Microsoft Windows Server 2003 R2");
                        break;
                }
                break;
            case 6:
                switch(os.dwMinorVersion){
                    case 0:
                        if(os.wProductType==VER_NT_WORKSTATION)/*

                        VER_NT_WORKSTATION是桌面系统 */
                            vname=_T("Microsoft Windows Vista");
                        else
                            vname=_T("Microsoft Windows Server 2008");          //服务器版本
                        break;
                    case 1:
                        if(os.wProductType==VER_NT_WORKSTATION)
                            vname=_T("Microsoft Windows 7");
                        else
                            vname=_T("Microsoft Windows Server 2008 R2");
                        break;
                }
                break;
            default:
                vname=_T("未知Windows操作系统");
        }
        return vname;
    }
    else
        return _T("获取系统版本失败！");
}
#endif

#ifdef __linux__
static char *const_getSystemVersion()
{
    static char *version = NULL;

    if (version) return version;

    FILE *fp = fopen("/etc/issue", "r");
    if (!fp) {
        version = "未知的Linux操作系统";
        return version;
    }
    version = g_new(char, 100);
    memset(version, 0, sizeof(char) * 100);
    fgets(version, 99, fp);
    fclose(fp);
    return version;
}
#endif


/* ==============================================
                Away.Const.Screen
   ============================================== */

/* Init && Final */
static void screen_Initialize(JSContextRef ctx, JSObjectRef object)
{
}

static void screen_Finalize(JSObjectRef object)
{
}

/* Width */
static JSValueRef screen_GetWidth(
    JSContextRef ctx,
    JSObjectRef  object,
    JSStringRef  propertyName,
    JSValueRef  *exception)
{
    int width;
    GdkScreen* screen;
    screen  = gdk_screen_get_default();
    width  = gdk_screen_get_width(screen);
    return JSValueMakeNumber(ctx, width);
}

/* Height */
static JSValueRef screen_GetHeight(
    JSContextRef ctx,
    JSObjectRef  object,
    JSStringRef  propertyName,
    JSValueRef  *exception)
{
    int height;
    GdkScreen* screen;
    screen  = gdk_screen_get_default();
    height  = gdk_screen_get_height(screen);
    return JSValueMakeNumber(ctx, height);
}

/* Create Const Object */
static JSClassRef Screen_ClassCreate(JSContextRef ctx)
{
    static JSClassRef screenClass = NULL;
    if (screenClass) {
        return screenClass;
    }

    JSStaticFunction screenStaticFunctions[] = {
        { NULL, 0, 0 },
    };

    JSStaticValue screenStaticValues[] = {
        { "Width",   screen_GetWidth,  NULL,  kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { "Height",  screen_GetHeight,  NULL,  kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { NULL, 0, 0, 0},
    };

    JSClassDefinition classdef = kJSClassDefinitionEmpty;
    classdef.className         = "AScreen";
    classdef.initialize        = screen_Initialize;
    classdef.finalize          = screen_Finalize;
    classdef.staticValues      = screenStaticValues;
    classdef.staticFunctions   = screenStaticFunctions;

    return screenClass = JSClassCreate(&classdef);
}
