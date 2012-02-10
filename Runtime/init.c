#include "init.h"
#include "global.h"
#include "common.h"
#include "server.h"
#include <string.h>

static void show_version()
{
    char version[1000] = {0};

    strcpy(version, "AwayFramework Runtime\n\nVersion ");
    strcat(version, VERSION_SHOW);
    show_message(version);
}

static void show_help()
{
    char help[1000] = {0};

    strcpy(help, "AwayFramework Runtime\n\n"
           "Usage: afr [OPTION]..[FILE]\n"
           "   -h, --help        show this help\n"
           "   -v, --version     show application version\n"
           "   -a appname        open appname\n"
           "   -f filename       open filename\n");
    show_message(help);
}

static void init_param(int argc, char *argv[])
{
    int g_argc, i;
    char **g_argv;

    g_argc = argc;
    g_argv = g_new(char *, argc);
    for (i = 0; i < argc; ++i) {
        g_argv[i] = g_strdup(argv[i]);
    }

    global_set("argc", (gpointer)g_argc);
    global_set("argv", g_argv);
}

static void init_environment()
{
    char **  argv;
    char *   program;
    char *   programpath;
    char *   tmpstr;


    argv = ((char **)global_get("argv"));

    /* Getting Program file name and Program Path */
    program = g_path_get_basename(argv[0]);
    programpath = g_path_get_dirname(argv[0]);
    if (!g_path_is_absolute(programpath)) {
        programpath = g_build_filename(g_get_current_dir(), programpath, NULL);
    }

    tmpstr = g_build_filename(programpath, "standalone", NULL);

    if (g_file_test(tmpstr, G_FILE_TEST_EXISTS)) {
        /* Standalone Version */
        global_set("userdir", programpath);
    } else {
        /* Install Version */
        global_set("userdir", g_build_filename(g_get_home_dir(), ".af", NULL));
    }

    global_set("program", program);
    global_set("programpath", programpath);
    g_free(tmpstr);
}

static int parse_param()
{
    int argc = (int)global_get("argc");
    char **argv = (char **)global_get("argv");
    char *filename, tmpstr[100];

    if (argc <= 1) {
        global_set("run_mode", AWAY_SHOW_LIST);
        global_set("filename", "away:home");
        return 0;
    }

    if (argc == 2) {
        if (strcmp("-v", argv[1]) == 0 || strcmp("--version", argv[1]) == 0) {
            show_version();
            return -1;
        }

        if (strcmp("-h", argv[1]) == 0 || strcmp("--help", argv[1]) == 0) {
            show_help();
            return -1;
        }
    }

    if (argc == 3) {
        /* Open App */
        if (strcmp("-a", argv[1]) == 0) {

            sprintf(tmpstr, "%s.afa", argv[2]);
            filename = g_build_filename(global_get("userdir"), "apps", argv[2], tmpstr, NULL);
            global_set("filename", filename);
            global_set("run_mode", (gpointer)AWAY_SHOW_APP);

            return 0;
        }
        /* Open File */
        if (strcmp("-f", argv[1]) == 0) {
            global_set("filename", g_strdup(argv[2]));
            global_set("run_mode", (gpointer)AWAY_SHOW_APP);

            return 0;
        }
    }

    /* Params Error! */
    show_help();

    return 1;
}

static void load_default_setting()
{
    global_set("width", (gpointer)400);
    global_set("height", (gpointer)300);
    global_set("resizeable", (gpointer)1);
    global_set("fullscreen", (gpointer)0);
    global_set("titlechangebycode", (gpointer)0);
    global_set("encode", "UTF-8");
    global_set("border", (gpointer)1);
}

static int load_configure()
{
    char *filename, *name, *path, *confpath, *tmpstr, *apphome, *htmlhome;
    GKeyFile *keyfile;
    int tmpint;
    GError *error = NULL;

    g_log("AFR", G_LOG_LEVEL_INFO, "Loading Configure...");

    filename = global_get("filename");
    apphome = global_get("apphome");
    name = af_file_get_name(filename);
    path = g_path_get_dirname(filename);

    g_log("AFR", G_LOG_LEVEL_INFO, "Loading File: %s", filename);

    keyfile = g_key_file_new();
    if (!g_key_file_load_from_file(keyfile, filename, G_KEY_FILE_NONE, NULL)) {
        show_errormessage("打开文件错误！");
        return 1;
    }

    /* Checking platform */
    tmpint = g_key_file_get_integer(keyfile, "app", "platform", NULL);
    if (!(tmpint&1)) {
        show_errormessage("此应用不支持Desktop平台！");
        return 1;
    }

    /* Checking Operaing System */
    tmpint = g_key_file_get_integer(keyfile, "app", "os", NULL);
#ifdef WIN32
    if (!(tmpint & 1)) {
        show_errormessage("此应用不支持Windows操作系统！");
#elif defined linux
    if (!(tmpint & 2)) {
        show_errormessage("此应用不支持Linux操作系统！");
#elif defined MACOS
    if (!(tmpint & 4)) {
        show_errormessage("此应用不支持MacOS操作系统！");
#else
    if (FALSE) {
#endif
        return 1;
    }

    /* Checking Version */
    tmpint = g_key_file_get_integer(keyfile, "app", "frameversion", NULL);
    if (tmpint > VERSION) {
        show_errormessage("此应用要求的运行库比您当前的高，请到http://af.awaysoft.com下载最新的运行库！");
        return 1;
    }

    /* Loading serial */
    tmpstr = g_key_file_get_string(keyfile, "app", "serial", NULL);
    if (!tmpstr || strcmp(tmpstr, "") == 0) {
        show_errormessage("此应用没有检测到标示符，请重新安装应用！");
        return 1;
    }
    global_set("serial", tmpstr);

    /* loading title */
    tmpstr = g_key_file_get_string(keyfile, "app", "title", NULL);
    if (!tmpstr) tmpstr = name;
    global_set("title", tmpstr);


    /* Setting HTMLHome */
    htmlhome = g_new(char, strlen(apphome) + 5);
    strcpy(htmlhome, apphome);
    strcat(htmlhome, "/app");
    global_set("htmlhome", htmlhome);

    /* Init Server */
    int port;
    port = init_server();
    if (port < 0) return 1;

    tmpstr = g_key_file_get_string(keyfile, "app", "index", NULL);
    if (!tmpstr) tmpstr = "index.html";
    if (strstr(tmpstr, "http:") == tmpstr) global_set("url", g_strdup(tmpstr));
    else {
        char *url = g_new(char, 100 + strlen(tmpstr));
        sprintf(url, "http://localhost:%d/%s", port, tmpstr);
        global_set("url", url);
    }

    tmpstr = g_key_file_get_string(keyfile, "app", "icon", NULL);
    if (!tmpstr) tmpstr = "main.ico";
    global_set("icon", g_build_filename(path, "res", tmpstr, NULL));

    g_key_file_free(keyfile);
    keyfile = g_key_file_new();
    /* Connect configure file name */
    tmpstr = g_new(char, strlen(name) + 6);
    strcpy(tmpstr, name);
    strcat(tmpstr, ".conf");
    confpath = g_build_filename(path, tmpstr, NULL);
    g_free(tmpstr);

    if (!g_key_file_load_from_file(keyfile, confpath, G_KEY_FILE_NONE, NULL)) {
        /* Configure File no found, using default value */
        load_default_setting();
        return 0;
    }

    g_log("AFR", G_LOG_LEVEL_INFO, "Loading Configure File: %s", confpath);

    /* loading width */
    tmpint = g_key_file_get_integer(keyfile, "conf", "width", NULL);
    if (!tmpint) tmpint = 400;
    global_set("width", (gpointer)tmpint);

    /* loading height */
    tmpint = g_key_file_get_integer(keyfile, "conf", "height", NULL);
    if (!tmpint) tmpint = 300;
    global_set("height", (gpointer)tmpint);

    /* loading x */
    tmpint = g_key_file_get_integer(keyfile, "conf", "x", &error);
    if (!tmpint && error && (error->code == G_KEY_FILE_ERROR_KEY_NOT_FOUND ||
                             error->code == G_KEY_FILE_ERROR_INVALID_VALUE)) tmpint = 10000;
    global_set("x", (gpointer)tmpint);
    if (error) {
        g_error_free(error);
        error = NULL;
    }

    /* loading y */
    tmpint = g_key_file_get_integer(keyfile, "conf", "y", &error);
    if (!tmpint && error && (error->code == G_KEY_FILE_ERROR_KEY_NOT_FOUND ||
                             error->code == G_KEY_FILE_ERROR_INVALID_VALUE)) tmpint = 10000;
    global_set("y", (gpointer)tmpint);
    if (error) {
        g_error_free(error);
        error = NULL;
    }

    /* loading resizeable */
    tmpint = g_key_file_get_integer(keyfile, "conf", "resizeable", &error);
    if (tmpint == 0 && error && (error->code == G_KEY_FILE_ERROR_KEY_NOT_FOUND ||
                                 error->code == G_KEY_FILE_ERROR_INVALID_VALUE)) tmpint = 1;
    global_set("resizeable", (gpointer)tmpint);
    if (error) {
        g_error_free(error);
        error = NULL;
    }

    /* loading fullscreen */
    tmpint = g_key_file_get_integer(keyfile, "conf", "fullscreen", NULL);
    global_set("fullscreen", (gpointer)tmpint);

    /* loading changebycode */
    tmpint = g_key_file_get_integer(keyfile, "conf", "titlechangebycode", NULL);
    global_set("titlechangebycode", (gpointer)tmpint);

    /* loading encode */
    tmpstr = g_key_file_get_string(keyfile, "conf", "encode", NULL);
    if (!tmpstr) tmpstr = "UTF-8";
    global_set("encode", tmpstr);

    /* loading border */
    tmpint = g_key_file_get_integer(keyfile, "conf", "border", &error);
    if (!tmpint && error && (error->code == G_KEY_FILE_ERROR_KEY_NOT_FOUND ||
                             error->code == G_KEY_FILE_ERROR_INVALID_VALUE)) tmpint = 1;
    global_set("border", (gpointer)tmpint);
    if (error) {
        g_error_free(error);
        error = NULL;
    }

    /* loading savelastposition */
    tmpint = g_key_file_get_integer(keyfile, "conf", "savelastposition", NULL);
    global_set("savelastposition", (gpointer)tmpint);

    g_key_file_free(keyfile);

    return 0;
}

static int load_default_configure() {
    char *filename, *name, *apphome, *htmlhome;
    runmode run_mode;

    g_log("AFR", G_LOG_LEVEL_INFO, "Loading Default Configure...");

    filename = global_get("filename");
    name = af_file_get_name(filename);
    run_mode = (runmode)global_get("run_mode");
    apphome = global_get("apphome");
    htmlhome = g_strdup(apphome);

    if (run_mode == AWAY_SHOW_LIST) {
        global_set("title", "Application List");
    } else {
        global_set("title", name);
    }

    global_set("htmlhome", htmlhome);
    global_set("url", filename);

    load_default_setting();
    return 0;
}

static int init_app() {
    char *filename;
    runmode run_mode;

    filename = global_get("filename");
    run_mode = (runmode)global_get("run_mode");

    /* RUN Application */
    if (run_mode == AWAY_SHOW_APP) {
        /* Check File exists */
        if (!g_file_test(filename, G_FILE_TEST_EXISTS)) {
            show_errormessage("文件不存在，请确认参数是否正确！");
            return 1;
        }

        char *ext = af_file_get_ext(filename);
        char *apphome = g_path_get_dirname(filename);

        global_set("apphome", apphome);

        if (strcmp(ext, "afa") == 0) {
            int return_var = load_configure();
            if (return_var != 0) return return_var;
        } else load_default_configure();

        /* Show Application List */
    } else {
        load_default_configure();
    }
    return 0;
}

int init(int argc, char *argv[]) {
    int return_int;

    /* Init Global Object */
    init_global();

    /* Init Params */
    init_param(argc, argv);

    /* Init Environment */
    init_environment();

    /* Parse Params */
    return_int = parse_param();
    if (return_int) return return_int;

    /* Init App Property  */
    return_int = init_app();
    if (return_int) return return_int;

    return 0;
}
