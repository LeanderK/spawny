#ifndef CONFIG_H
#define CONFIG_H

#cmakedefine SERVER_SOCKET @SERVER_SOCKET@
#cmakedefine SERVER_DEBUG_SOCKET @SERVER_DEBUG_SOCKET@

#cmakedefine PAM_SERVICE_GREETER @PAM_SERVICE_GREETER@
#cmakedefine PAM_SERVICE @PAM_SERVICE@

#cmakedefine FALLBACK_GREETER @FALLBACK_GREETER@

#cmakedefine PACKAGE_DATA_DIR @PACKAGE_DATA_DIR@
#cmakedefine PACKAGE_LIB_DIR @PACKAGE_LIB_DIR@
#cmakedefine PACKAGE_BIN_DIR @PACKAGE_BIN_DIR@
#cmakedefine PACKAGE_CONFIG @PACKAGE_CONFIG@

#endif // CONFIG_H
