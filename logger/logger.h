
// Smart logger library for C/C++
/// Alex V. Bobryshev 2009-2015   explosion@ukr.net

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, 
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
// OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
// OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
// THE POSSIBILITY OF SUCH DAMAGE.


// LOG MACROSES:
// $(yyyy) $(yy) - year
// $(MM) $(M) - month
// $(dd) $(d) - day
// $(hh) $(h) - hour
// $(mm) $(m) - min
// $(ss) $(s) - sec
// $(ttt) $(t) - milliseconds
// $(V) $(v) - verbose
// $(MODULE) - current module name full
// $(module) - current module name short
// $(function) - function name
// $(line) - line number
// $(srcfile) - source file
// $(PID) - process ID
// $(TID) - thread ID


// CONFIG PATH & NAMES MACROSES:
// $(SYSTEMPATH) - path to Windows\System
// $(WINDOWSPATH) - path to Windows
// $(COMMONAPPDATA) - path to common application data
// $(USERAPPDATA) - path to current user application data
// $(DESKTOPDIR) - path to desktop
// $(TEMPPATH) - path to temp directory
// $(CURRENTDIR) - current process directory
// $(EXEDIR) - path to process EXE
// $(EXEFILENAME) - process EXE file name without extension
// $(EXEFULLFILENAME) - process EXE file name with extension

// only if LOG_USE_MODULEDEFINITION
// $(MODULEDIR) - module directory
// $(MODULEFILENAME) - module file name without extension
// $(MODULEFULLFILENAME) - module file name with extension


// Sample INI file

// [logger]
// LogPath=$(USERAPPDATA)\$(EXEFILENAME)\Log
// Verbose=255
// HeaderFormat=$(V)|$(dd).$(MM).$(yyyy)|$(hh):$(mm):$(ss).$(ttt) ($(module)!$(function))  
// LogFileName=$(EXEFILENAME).log
// LogSysInfo=1
// ScrollFileCount=70
// ScrollFileSize=1638400
// ScrollFileEveryRun=1
// RegistryConfigPath=HKCU\Software\$(EXEFILENAME)\Logging


#ifndef __LOGGER_HEADER
#define __LOGGER_HEADER

#pragma once

//////////////////////////////////////////////////////////////
//////////////  CONFIGURATION SECTION BEGIN  /////////////////

/// USER CONFIGURATION

/// Turn Logger on or off
#ifndef LOG_ENABLED
#	define LOG_ENABLED 1
#endif //LOG_ENABLED

/// Enable Logging only in debug version. In release it will be turned off
/// Otherwise it will be enabled in both release and debug versions
#ifndef LOG_ONLY_DEBUG
#	define LOG_ONLY_DEBUG 0
#endif //LOG_ONLY_DEBUG

/// Show system info on start session
#ifndef LOG_USE_SYSTEMINFO
#	define LOG_USE_SYSTEMINFO 1
#endif //LOG_USE_SYSTEMINFO

/// Use module definition system. It need for macros '$(module)' works, modules dumping and auto-debugging
/// If auto debugging is on it turns on automatically
/// On linux you need to define linker flag -ldl
#ifndef LOG_USE_MODULEDEFINITION
#	define LOG_USE_MODULEDEFINITION 1
#endif //LOG_USE_MODULEDEFINITION

/// Use auto debugging. It need for stack trace
/// On linux it is strongly recommended to build with -rdynamic linker key
#ifndef LOG_AUTO_DEBUGGING
#	define LOG_AUTO_DEBUGGING 1
#endif //LOG_AUTO_DEBUGGING

/// Catch unhandled exceptions, Logging them and creating mini dumps
/// If auto debugging enabled it show stack trace
#ifndef LOG_UNHANDLED_EXCEPTIONS
#	define LOG_UNHANDLED_EXCEPTIONS 1
#endif //LOG_UNHANDLED_EXCEPTIONS

/// Configure Log from registry. Registry parameters are more priority that configuration from program
/// Registry path must be configured before first access to Log
#ifndef LOG_CONFIGURE_FROM_REGISTRY
#	define LOG_CONFIGURE_FROM_REGISTRY 0
#endif //LOG_CONFIGURE_FROM_REGISTRY

/// Enable configuration from INI file
#ifndef LOG_INI_CONFIGURATION
#	define LOG_INI_CONFIGURATION 1
#endif //LOG_INI_CONFIGURATION

/// Paths which will be used for find INI file. First defined paths are more prioritized
#ifndef LOG_DEFAULT_INI_PATHS
#	define LOG_DEFAULT_INI_PATHS "$(EXEDIR)/$(EXEFILENAME).log.ini;$(MODULEDIR)/$(MODULEFILENAME).log.ini;$(CURRENTDIR)/$(EXEFILENAME).log.ini;$(CURRENTDIR)/$(MODULEFILENAME).log.ini"
#endif //LOG_DEFAULT_INI_PATHS

#ifndef LOG_REGISTRY_DEFAULT_KEY
#	define LOG_REGISTRY_DEFAULT_KEY "HKCU\\Software\\$(EXEFILENAME)\\Logging"
#endif //LOG_REGISTRY_DEFAULT_KEY

/// Create directory for Log
#ifndef LOG_CREATE_DIRECTORY
#	define LOG_CREATE_DIRECTORY 1
#endif //LOG_CREATE_DIRECTORY

/// Enable using RTTI in Log
#ifndef LOG_RTTI_ENABLED
#	define LOG_RTTI_ENABLED 1
#endif //LOG_RTTI_ENABLED

/// Use one shared logging object for process
#ifndef LOG_SHARED
#	define LOG_SHARED 1
#endif //LOG_SHARED

/// Enable or disable logger configuration compiler warnings
#ifndef LOG_COMPILER_WARNINGS
#	define LOG_COMPILER_WARNINGS 1
#endif //LOG_COMPILER_WARNINGS

/// Get logging module code from external DLL
#ifndef LOG_USE_DLL
#	define LOG_USE_DLL 1
#endif //LOG_USE_DLL

#ifndef LOG_MULTITHREADED
#	define LOG_MULTITHREADED 1
#endif //LOG_MULTITHREADED

// Include CONFIG.H for platform detection (for posix-based systems)
// need to detect: unistd.h sys/syscall.h sys/utsname.h pwd.h pthread sys/mman.h
// WARNING: if some of this dependencies is not specified in autoconf, turn LOG_USE_CONFIG_H to 0!
#ifndef LOG_USE_CONFIG_H
#	define LOG_USE_CONFIG_H  0
#endif //LOG_USE_CONFIG_H

//////////////////////////////////////////////////////////////
/// THIN CONFIGURATION
/// DOES NOT NEED TO MODIFICATION IN MOST CASES

/// Open and close file at each write. Causes more stable writes on crashes but can produce performance issue
#ifndef LOG_FLUSH_FILE_EVERY_WRITE
#	define LOG_FLUSH_FILE_EVERY_WRITE 0
#endif //LOG_FLUSH_FILE_EVERY_WRITE

/// Use debug macro and mechanisms in log
#ifndef LOG_CHECKED
#	define LOG_CHECKED 1
#endif //LOG_CHECKED

/// Show message on fatal application crash and do not show window 'this program cause error... send report...'
/// Do not recommended - in this case logger injects to business logic of app crash workflow
#ifndef LOG_SHOW_MESSAGE_ON_FATAL_CRASH
#	define LOG_SHOW_MESSAGE_ON_FATAL_CRASH 1
#endif //LOG_SHOW_MESSAGE_ON_FATAL_CRASH

/// Detection module name by instruction pointer in stacktrace, if it was not found in stack frames
/// Does not recommended, it is really slow and does not need most cases
#ifndef LOG_STACKTRACE_DETECT_MODNAME_IF_NOT_FOUND
#	define LOG_STACKTRACE_DETECT_MODNAME_IF_NOT_FOUND 0
#endif //LOG_STACKTRACE_DETECT_MODNAME_IF_NOT_FOUND

/// Release logger after dump creation before application will crash. Used only if LOG_UNHANDLED_EXCEPTIONS was set.
/// Set this value to 0 can cause log file flush issues but may be useful if you using debugger AFTER crash
#ifndef LOG_RELEASE_ON_APP_CRASH
#	define LOG_RELEASE_ON_APP_CRASH 1
#endif //LOG_RELEASE_ON_APP_CRASH

/// Use modules cache for detect module name by address. Used only if LOG_USE_MODULEDEFINITION.
/// Cache is optimizing performance but it is not support modules unload. If you write system-trick tool or application
/// which very often load-unload DLLs, maybe you need to turn off modules cache
#ifndef LOG_USE_MODULES_CACHE
#	define LOG_USE_MODULES_CACHE 1
#endif //LOG_RELEASE_ON_APP_CRASH

/// Cache headers macro. Turned on by default. Turn off if you have problems with macro header format
#ifndef LOG_USE_MACRO_HEADER_CACHE
#	define LOG_USE_MACRO_HEADER_CACHE 1
#endif //LOG_USE_MACRO_HEADER_CACHE

/// Process macro in logging text same as in header. Cause performance reducing
#ifndef LOG_PROCESS_MACRO_IN_LOG_TEXT
#	define LOG_PROCESS_MACRO_IN_LOG_TEXT 0
#endif //LOG_PROCESS_MACRO_IN_LOG_TEXT

/// Log works but does not create or write to file. Used only for performance analysis, do not use in real applications!
#ifndef LOG_TEST_DO_NOT_WRITE_FILE
#	define LOG_TEST_DO_NOT_WRITE_FILE 0
#endif //LOG_TEST_DO_NOT_WRITE_FILE

/// Use SEH. Windows only
#ifndef LOG_USE_SEH
#	define LOG_USE_SEH	1
#endif //LOG_USE_SEH

/// Use ANSI for varargs
/// Must be set for windows
#define ANSI

////////////////  CONFIGURATION SECTION END  /////////////////
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//////////////////// Platform detection //////////////////////


#if defined(unix) || defined(__unix__) || defined(__unix)
#	define LOG_PLATFORM_POSIX_BASED

#	if defined(__CYGWIN__) && !defined(_WIN32)
#		define LOG_PLATFORM_CYGWIN
#	endif //defined(__CYGWIN__) && !defined(_WIN32)

#   if !defined(__linux__) && !defined(__linux) && !defined(linux) && !defined(__sun__) && !defined(__sun) && !defined(__CYGWIN__) && !defined(__APPLE__) && !defined(_WIN32)
#       define LOG_PLATFORM_UNIX
#		include <sys/param.h>

#		if defined(BSD)
#			define LOG_PLATFORM_BSD
#		endif //BSD

#		if defined(__FreeBSD__)
#			define LOG_PLATFORM_FREEBSD
#		endif //__FreeBSD__

#		if defined(__NetBSD__)
#			define LOG_PLATFORM_NETBSD
#		endif //__NetBSD__

#		if defined(__DragonFly__)
#			define LOG_PLATFORM_DRAGONFLYBSD
#		endif //__DragonFly__

#   endif //!defined(__linux__) && !defined(__linux) && !defined(linux) && !defined(__sun__) && !defined(__sun) && !defined(__CYGWIN__) && !defined(__APPLE__) && !defined(_WIN32)


#endif //defined(unix) || defined(__unix__) || defined(__unix)

#if !defined(LOG_PLATFORM_UNIX) && defined(LOG_PLATFORM_POSIX_BASED)
#   if defined(__linux__) || defined(__linux) || defined(__gnu_linux) || defined(linux)
#       define LOG_PLATFORM_LINUX
#   endif //defined(__linux__) || defined(__linux) || defined(__gnu_linux) || defined(linux)
#endif //!defined LOG_PLATFORM_UNIX && defined(LOG_PLATFORM_POSIX_BASED)

#if defined(__APPLE__) && defined(__MACH__)
#	define LOG_PLATFORM_POSIX_BASED
#   define LOG_PLATFORM_MAC

#	include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
#	define LOG_PLATFORM_IPHONE
#	define LOG_PLATFORM_IPHONE_SIMULATOR
#elif TARGET_OS_IPHONE == 1
#	define LOG_PLATFORM_IPHONE
#elif TARGET_OS_MAC == 1
#	define LOG_PLATFORM_MACOSX
#endif

#endif //defined(__APPLE__) && defined(__MACH__)

#if defined(LOG_PLATFORM_POSIX_BASED) && (defined(__sun__) || defined(__sun) || defined(__SunOS) || defined(sun))
#	define LOG_PLATFORM_POSIX_BASED
#   define LOG_PLATFORM_SOLARIS
#endif //defined(LOG_PLATFORM_POSIX_BASED) && (defined(__sun__) || defined(__sun) || defined(__SunOS) || defined(sun))

#ifdef _WIN32
#	undef LOG_PLATFORM_POSIX_BASED // it can be set if you use CYGWIN with GCC for Windows target
#	undef LOG_PLATFORM_CYGWIN
#	undef LOG_PLATFORM_UNIX

#   define LOG_PLATFORM_WINDOWS
#endif //_WIN32

////////////////// Platform detection end ////////////////////
//////////////////////////////////////////////////////////////

// Compiler detection

#ifdef _MSC_VER
#	define LOG_COMPILER_MSVC
#endif //_MSC_VER

#ifdef __GNUC__
#	define LOG_COMPILER_GCC
#endif //__GNUC__

#ifdef __IBMCPP__
#	define LOG_COMPILER_IBM
#endif //__IBMCPP__

#if defined(__ICC) || defined(__INTEL_COMPILER)
#	define LOG_COMPILER_ICC
#endif //defined(__ICC) || defined(__INTEL_COMPILER)

#if defined(__SUNPRO_CC) || defined(__SUNPRO_C)
#	define LOG_COMPILER_SOLARIS
#endif //defined(__SUNPRO_CC) || defined(__SUNPRO_C)

#if defined(__MINGW32__) || defined(__MINGW64__)
#	define LOG_COMPILER_MINGW
#endif //defined(__MINGW32__) || defined(__MINGW64__)

//////////////////////////////////////////////////////////////
//////////// POSIX BASED PLATFORM CONFIGURATION //////////////

#ifdef LOG_PLATFORM_POSIX_BASED

#if !LOG_USE_CONFIG_H

// User can change this section if autoconf is not used
#	define LOG_HAVE_UNISTD_H
#	define LOG_HAVE_SYS_SYSCALL_H
#	define LOG_HAVE_SYS_UTSNAME_H
#	define LOG_HAVE_PWD_H
#	define LOG_HAVE_PTHREAD
#	define LOG_HAVE_SYS_MMAN_H
#	define LOG_HAVE_SYS_SYSINFO_H

// Platform-based detection
#	ifdef LOG_PLATFORM_CYGWIN
#		undef LOG_HAVE_SYS_SYSCALL_H
#	endif //LOG_PLATFORM_CYGWIN

#else //!LOG_USE_CONFIG_H

#	ifdef HAVE_UNISTD_H
#		define LOG_HAVE_UNISTD_H
#	endif //HAVE_UNISTD_H

#	ifdef HAVE_SYS_SYSCALL_H
#		define LOG_HAVE_SYS_SYSCALL_H
#	endif //HAVE_SYS_SYSCALL_H

#	ifdef HAVE_SYS_UTSNAME_H
#		define LOG_HAVE_SYS_UTSNAME_H
#	endif //HAVE_SYS_UTSNAME_H

#	ifdef HAVE_PWD_H
#		define LOG_HAVE_PWD_H
#	endif //HAVE_PWD_H

#	ifdef HAVE_PTHREAD
#		define LOG_HAVE_PTHREAD
#	endif //HAVE_PTHREAD

#	ifdef HAVE_SYS_MMAN_H
#		define LOG_HAVE_SYS_MMAN_H
#	endif //HAVE_SYS_MMAN_H

#	ifdef HAVE_SYS_SYSINFO_H
#		define LOG_HAVE_SYS_SYSINFO_H
#	endif //HAVE_SYS_SYSINFO_H

#endif //LOG_USE_CONFIG_H

#endif //LOG_PLATFORM_POSIX_BASED

////////// POSIX BASED PLATFORM CONFIGURATION END ////////////
//////////////////////////////////////////////////////////////

#if (!defined(LOG_PLATFORM_WINDOWS) && !defined(LOG_PLATFORM_POSIX_BASED)) || (defined(LOG_PLATFORM_WINDOWS) && defined(LOG_PLATFORM_POSIX_BASED))
#	error "Undefined or invalid platform detected"
#endif //(!defined(LOG_PLATFORM_WINDOWS) && !defined(LOG_PLATFORM_POSIX_BASED)) || (defined(LOG_PLATFORM_WINDOWS) && defined(LOG_PLATFORM_POSIX_BASED))

#ifdef LOG_PLATFORM_POSIX_BASED
#	define LOG_CDECL
#endif //LOG_PLATFORM_POSIX_BASED

#ifdef LOG_PLATFORM_WINDOWS
#	define LOG_CDECL	__cdecl
#endif //LOG_PLATFORM_WINDOWS

#ifdef LOG_COMPILER_MSVC
#	define LOG_FMT_I64	"%I64d"
#else //LOG_COMPILER_MSVC
#	define LOG_FMT_I64	"%lld"
#endif //LOG_COMPILER_MSVC


#if LOG_ONLY_DEBUG
#	ifdef DEBUG
#		undef LOG_ENABLED
#		define LOG_ENABLED 0
#	endif //DEBUG
#endif //LOG_ONLY_DEBUG

#if !LOG_ENABLED
#	define LOG_INFO(...)
#	define LOG_WARNING(...)
#	define LOG_DEBUG(...)
#	define LOG_ERROR(...)
#	define LOG_FATAL(...)

#	define LOG_BINARY_INFO(p,stack_frame)
#	define LOG_BINARY_WARNING(p,stack_frame)
#	define LOG_BINARY_DEBUG(p,stack_frame)
#	define LOG_BINARY_ERROR(p,stack_frame)
#	define LOG_BINARY_FATAL(p,stack_frame)

#	define LOG_EXCEPTION_DEBUG(e)
#	define LOG_EXCEPTION_WARNING(e)
#	define LOG_EXCEPTION_INFO(e) 
#	define LOG_EXCEPTION_ERROR(e)
#	define LOG_EXCEPTION_FATAL(e)

#	define LOG_MODULES_DEBUG 
#	define LOG_MODULES_INFO 
#	define LOG_MODULES_WARNING 
#	define LOG_MODULES_ERROR 
#	define LOG_MODULES_FATAL 

#	define LOG_STACKTRACE_DEBUG 
#	define LOG_STACKTRACE_INFO
#	define LOG_STACKTRACE_WARNING
#	define LOG_STACKTRACE_ERROR
#	define LOG_STACKTRACE_FATAL

#	define DEFINE_LOGGER

#	define LOG_SET_VERBOSE_LEVEL(l)
#	define LOG_SET_REG_CONFIG_PATH(p)
#	define LOG_SET_INI_CONFIG_PATHS(p)

#	define LOGGER_VERBOSE_FATAL        0
#	define LOGGER_VERBOSE_ERROR        0
#	define LOGGER_VERBOSE_INFO         0
#	define LOGGER_VERBOSE_WARNING      0
#	define LOGGER_VERBOSE_DEBUG        0
#	define LOGGER_VERBOSE_OPTIMAL      0
#	define LOGGER_VERBOSE_MUTE         0
#	define LOGGER_VERBOSE_FATAL_ERROR  0
#	define LOGGER_VERBOSE_ALL          0
#	define LOGGER_VERBOSE_NORMAL       0


#else // LOG_ENABLED

#	ifdef ANSI 
#		include <stdarg.h>
  int average( int first, ... );
#	else //ANSI
#		include <vadefs.h>
#		include <varargs.h>
  int average( va_list );
#	endif //ANSI


#if LOG_SHARED && !defined(LOG_PLATFORM_WINDOWS)
#	include <sys/mman.h>
#	define SH_MEM_READ	1
#	define SH_MEM_WRITE	2
#	define SH_MEM_EXEC	4
#endif //LOG_SHARED && !defined(LOG_PLATFORM_WINDOWS)

#if LOG_USE_MODULEDEFINITION

#ifdef LOG_COMPILER_MSVC

#ifdef __cplusplus
extern "C"
#endif//__cplusplus
void* _ReturnAddress(void);

#pragma intrinsic(_ReturnAddress)

#pragma optimize ( "", off )

#endif //LOG_COMPILER_MSVC

#if defined(LOG_COMPILER_GCC) || defined(LOG_COMPILER_MINGW)
#   pragma GCC push_options
#   pragma GCC optimize ("O0")
#endif // defined(LOG_COMPILER_GCC) || defined(LOG_COMPILER_MINGW)


static void* logging_get_caller_address()
{
#ifdef LOG_COMPILER_MSVC
    return _ReturnAddress();
#endif //LOG_COMPILER_MSVC

#if defined(LOG_COMPILER_GCC) || defined(LOG_COMPILER_MINGW) || defined(LOG_COMPILER_ICC)
    return __builtin_return_address(0);
#endif // defined(LOG_COMPILER_GCC) || defined(LOG_COMPILER_MINGW) || defined(LOG_COMPILER_ICC)
}

#ifdef LOG_COMPILER_MSVC
#pragma optimize ( "", on )
#endif //LOG_COMPILER_MSVC

#if defined(LOG_COMPILER_GCC) || defined(LOG_COMPILER_MINGW)
#   pragma GCC pop_options
#endif //defined(LOG_COMPILER_GCC) || defined(LOG_COMPILER_MINGW)

#endif //LOG_USE_MODULEDEFINITION


// Logger verbose level both for C++ and C code

#if defined(__cplusplus) && (!LOG_USE_DLL || defined(LOG_THIS_IS_DLL))
namespace logging {
#endif //defined(__cplusplus) && (!LOG_USE_DLL || defined(LOG_THIS_IS_DLL))

enum logger_verbose_level
{
	logger_verbose_fatal = 1,
	logger_verbose_error = 2,
	logger_verbose_warning = 4,
	logger_verbose_info = 8,
	logger_verbose_debug = 16,

	logger_verbose_fatal_error = logger_verbose_fatal | logger_verbose_error,
	logger_verbose_all = logger_verbose_fatal | logger_verbose_error | logger_verbose_warning |
				logger_verbose_info | logger_verbose_debug,

	logger_verbose_normal = logger_verbose_fatal | logger_verbose_error | logger_verbose_warning | logger_verbose_info,
	logger_verbose_mute = 0,

#ifdef _DEBUG
	logger_verbose_optimal = logger_verbose_normal | logger_verbose_debug
#else //_DEBUG
	logger_verbose_optimal = logger_verbose_normal
#endif //_DEBUG
};

#if defined(__cplusplus) && (!LOG_USE_DLL || defined(LOG_THIS_IS_DLL))
}; //namespace logging
#endif //defined(__cplusplus) && (!LOG_USE_DLL || defined(LOG_THIS_IS_DLL))

#if defined(__cplusplus) && (!LOG_USE_DLL || defined(LOG_THIS_IS_DLL))
#	define LOGGER_VERBOSE_FATAL        logging::logger_verbose_fatal
#	define LOGGER_VERBOSE_ERROR        logging::logger_verbose_error
#	define LOGGER_VERBOSE_INFO         logging::logger_verbose_info
#	define LOGGER_VERBOSE_WARNING      logging::logger_verbose_warning
#	define LOGGER_VERBOSE_DEBUG        logging::logger_verbose_debug
#	define LOGGER_VERBOSE_OPTIMAL      logging::logger_verbose_optimal
#	define LOGGER_VERBOSE_MUTE         logging::logger_verbose_mute
#	define LOGGER_VERBOSE_FATAL_ERROR  logging::logger_verbose_fatal_error
#	define LOGGER_VERBOSE_ALL          logging::logger_verbose_all
#	define LOGGER_VERBOSE_NORMAL       logging::logger_verbose_normal
#else //defined(__cplusplus) && (!LOG_USE_DLL || defined(LOG_THIS_IS_DLL))
#	define LOGGER_VERBOSE_FATAL        logger_verbose_fatal
#	define LOGGER_VERBOSE_ERROR        logger_verbose_error
#	define LOGGER_VERBOSE_INFO         logger_verbose_info
#	define LOGGER_VERBOSE_WARNING      logger_verbose_warning
#	define LOGGER_VERBOSE_DEBUG        logger_verbose_debug
#	define LOGGER_VERBOSE_OPTIMAL      logger_verbose_optimal
#	define LOGGER_VERBOSE_MUTE         logger_verbose_mute
#	define LOGGER_VERBOSE_FATAL_ERROR  logger_verbose_fatal_error
#	define LOGGER_VERBOSE_ALL          logger_verbose_all
#	define LOGGER_VERBOSE_NORMAL       logger_verbose_normal
#endif //defined(__cplusplus) && (!LOG_USE_DLL || defined(LOG_THIS_IS_DLL))


// precompiled configuration checking

#	ifdef LOG_PLATFORM_CYGWIN

#		if LOG_UNHANDLED_EXCEPTIONS
#			if LOG_COMPILER_WARNINGS

#				ifdef LOG_COMPILER_MSVC
#					pragma message("LOGGER: CYGWIN is not support LOG_UNHANDLED_EXCEPTIONS")
#				else //LOG_COMPILER_MSVC
#					warning("LOGGER: CYGWIN is not support LOG_UNHANDLED_EXCEPTIONS")
#				endif //LOG_COMPILER_MSVC

#			endif //LOG_COMPILER_WARNINGS

#			undef LOG_UNHANDLED_EXCEPTIONS
#			define LOG_UNHANDLED_EXCEPTIONS 0
#		endif //LOG_UNHANDLED_EXCEPTIONS

#		if LOG_AUTO_DEBUGGING
#			if LOG_COMPILER_WARNINGS

#				ifdef LOG_COMPILER_MSVC
#					pragma message("LOGGER: CYGWIN is not support LOG_AUTO_DEBUGGING")
#				else //LOG_COMPILER_MSVC
#					warning("LOGGER: CYGWIN is not support LOG_AUTO_DEBUGGING")
#				endif //LOG_COMPILER_MSVC

#			endif //LOG_COMPILER_WARNINGS

#			undef LOG_AUTO_DEBUGGING
#			define LOG_AUTO_DEBUGGING 0
#		endif //LOG_AUTO_DEBUGGING

#		if LOG_USE_MODULEDEFINITION
#			if LOG_COMPILER_WARNINGS

#				ifdef LOG_COMPILER_MSVC
#					pragma message("LOGGER: CYGWIN is not support LOG_USE_MODULEDEFINITION")
#				else //LOG_COMPILER_MSVC
#					warning("LOGGER: CYGWIN is not support LOG_USE_MODULEDEFINITION")
#				endif //LOG_COMPILER_MSVC

#			endif //LOG_COMPILER_WARNINGS

#			undef LOG_USE_MODULEDEFINITION
#			define LOG_USE_MODULEDEFINITION 0
#		endif //LOG_USE_MODULEDEFINITION

#	endif //LOG_PLATFORM_CYGWIN

#	if LOG_SHARED && defined(LOG_PLATFORM_POSIX_BASED) && !defined(LOG_HAVE_SYS_MMAN_H)
#		if LOG_COMPILER_WARNINGS

#			ifdef LOG_COMPILER_MSVC
#				pragma message("LOGGER: System has no sys/mman.h - logger cannot be shared (LOG_SHARED)")
#			else //LOG_COMPILER_MSVC
#				warning("LOGGER: System has no sys/mman.h - logger cannot be shared (LOG_SHARED)")
#			endif //LOG_COMPILER_MSVC

#		endif //LOG_COMPILER_WARNINGS
#		undef LOG_SHARED
#		define LOG_SHARED 0
#	endif //LOG_SHARED && defined(LOG_PLATFORM_POSIX_BASED) && !defined(LOG_HAVE_SYS_MMAN_H)

#	if LOG_MULTITHREADED && defined (LOG_PLATFORM_POSIX_BASED) && !defined(LOG_HAVE_PTHREAD)
#		if LOG_COMPILER_WARNINGS
// warning only for posix-based systems, no MSVC
#			warning("LOGGER: System has no pthread - logger cannot be multithreaded (LOG_MULTITHREADED)")
#		endif //LOG_COMPILER_WARNINGS

#		undef LOG_MULTITHREADED
#		define LOG_MULTITHREADED 0
#	endif //LOG_MULTITHREADED && defined (LOG_PLATFORM_POSIX_BASED) && !defined(LOG_HAVE_PTHREAD)

#	if LOG_UNHANDLED_EXCEPTIONS && !LOG_AUTO_DEBUGGING
#		if LOG_COMPILER_WARNINGS

#			ifdef LOG_COMPILER_MSVC
#				pragma message("LOGGER: Specified LOG_UNHANDLED_EXCEPTIONS without LOG_AUTO_DEBUGGING. Force set LOG_AUTO_DEBUGGING = 1")
#			else //LOG_COMPILER_MSVC
#				warning("LOGGER: Specified LOG_UNHANDLED_EXCEPTIONS without LOG_AUTO_DEBUGGING. Force set LOG_AUTO_DEBUGGING = 1")
#			endif //LOG_COMPILER_MSVC

#		endif //LOG_COMPILER_WARNINGS

#		undef LOG_AUTO_DEBUGGING
#		define LOG_AUTO_DEBUGGING 1
#	endif //LOG_UNHANDLED_EXCEPTIONS && !LOG_AUTO_DEBUGGING

#	if LOG_AUTO_DEBUGGING && !LOG_USE_MODULEDEFINITION
#		if LOG_COMPILER_WARNINGS

#			ifdef LOG_COMPILER_MSVC
#				pragma message("LOGGER: Specified LOG_AUTO_DEBUGGING without LOG_USE_MODULEDEFINITION. Force set LOG_USE_MODULEDEFINITION = 1")
#			else //LOG_COMPILER_MSVC
#				warning("LOGGER: Specified LOG_AUTO_DEBUGGING without LOG_USE_MODULEDEFINITION. Force set LOG_USE_MODULEDEFINITION = 1")
#			endif //LOG_COMPILER_MSVC

#		endif//LOG_COMPILER_WARNINGS

#		undef LOG_USE_MODULEDEFINITION
#		define LOG_USE_MODULEDEFINITION 1
#	endif //LOG_AUTO_DEBUGGING && !LOG_USE_MODULEDEFINITION

#	if LOG_RELEASE_ON_APP_CRASH && !LOG_UNHANDLED_EXCEPTIONS
#		if LOG_COMPILER_WARNINGS

#			ifdef LOG_COMPILER_MSVC
#				pragma message("LOGGER: Log configuration issue. LOG_RELEASE_ON_APP_CRASH is enabled but LOG_UNHANDLED_EXCEPTIONS is disabled. No actions will be taken")
#			else //LOG_COMPILER_MSVC
#				warning("LOGGER: Log configuration issue. LOG_RELEASE_ON_APP_CRASH is enabled but LOG_UNHANDLED_EXCEPTIONS is disabled. No actions will be taken")
#			endif //LOG_COMPILER_MSVC

#		endif //LOG_COMPILER_WARNINGS
#	endif //LOG_RELEASE_ON_APP_CRASH && !LOG_UNHANDLED_EXCEPTIONS

#	if LOG_UNHANDLED_EXCEPTIONS && !LOG_FLUSH_FILE_EVERY_WRITE && !LOG_RELEASE_ON_APP_CRASH
#		if LOG_COMPILER_WARNINGS

#			ifdef LOG_COMPILER_MSVC
#				pragma message("LOGGER: Log configuration issue. LOG_FLUSH_FILE_EVERY_WRITE is unset and LOG_RELEASE_ON_APP_CRASH is unset. You can obtain corrupted log file if process will crash")
#			else //LOG_COMPILER_MSVC
#				warning("LOGGER: Log configuration issue. LOG_FLUSH_FILE_EVERY_WRITE is unset and LOG_RELEASE_ON_APP_CRASH is unset. You can obtain corrupted log file if process will crash")
#			endif //LOG_COMPILER_MSVC

#		endif //LOG_COMPILER_WARNINGS
#	endif //LOG_UNHANDLED_EXCEPTIONS && !LOG_FLUSH_FILE_EVERY_WRITE && !LOG_RELEASE_ON_APP_CRASH

#	if defined(LOG_PLATFORM_WINDOWS) && LOG_USE_SEH && !defined(LOG_COMPILER_MSVC)
#		if LOG_COMPILER_WARNINGS
#			warning("LOGGER: LOG_USE_SEH can be used only with Microsoft Visual C++ compiler")
#		endif //LOG_COMPILER_WARNINGS

#		undef LOG_USE_SEH
#		define LOG_USE_SEH 0

#	endif //defined(LOG_PLATFORM_WINDOWS) && LOG_USE_SEH && !defined(LOG_COMPILER_MSVC)





#	define INCLUDE_DBGHELP 0

#	if (LOG_AUTO_DEBUGGING || LOG_UNHANDLED_EXCEPTIONS) && !INCLUDE_DBGHELP
#		undef INCLUDE_DBGHELP
#		define INCLUDE_DBGHELP 1
#	endif //(LOG_AUTO_DEBUGGING || LOG_UNHANDLED_EXCEPTIONS) && !INCLUDE_DBGHELP

#if LOG_USE_MODULEDEFINITION
#	define LOG_GET_CALLER_ADDR  logging_get_caller_address()
#else
#	define LOG_GET_CALLER_ADDR  0L
#endif //LOG_USE_MODULEDEFINITION


#if defined(__cplusplus) && (!LOG_USE_DLL || defined(LOG_THIS_IS_DLL))

#ifdef LOG_COMPILER_MSVC
#	pragma warning(disable:4996)
#endif //LOG_COMPILER_MSVC

#	include <stdio.h>
#	include <string>
#	include <sstream>
#	include <ostream>
#	include <fstream>
#	include <map>
#	include <vector>
#	include <algorithm>
#	include <time.h>
#	include <sys/types.h>
#	include <sys/timeb.h>
#	include <list>
#	include <string.h>
#	include <stdlib.h>

#if LOG_RTTI_ENABLED
#   include <typeinfo>
#endif //LOG_RTTI_ENABLED

#if LOG_INI_CONFIGURATION
#	include <ctype.h>
#endif //LOG_INI_CONFIGURATION

#if LOG_CHECKED
#	include <assert.h>
#endif //LOG_CHECKED

#ifdef LOG_PLATFORM_WINDOWS
#	include <windows.h>
#	include <Sddl.h>
#	include <Shlwapi.h>
#	include <ShlObj.h>
#	pragma comment(lib,"advapi32.lib")
#	pragma comment(lib,"shlwapi.lib")
#else //LOGGER_PLATFORM_WINDOWS

#	ifdef LOG_HAVE_UNISTD_H
#   include <unistd.h>
#	endif //LOG_HAVE_UNISTD_H

#   include <dirent.h>
#   include <fnmatch.h>
#   include <sys/stat.h>
#   include <sys/time.h>
#   include <errno.h>

#ifdef LOG_HAVE_SYS_SYSCALL_H
#	include <sys/syscall.h>
#endif //LOG_HAVE_SYS_SYSCALL_H


#if LOG_MULTITHREADED && defined(LOG_HAVE_PTHREAD)
#	include <pthread.h>
#endif //LOG_MULTITHREADED && defined(LOG_HAVE_PTHREAD)

#   if LOG_USE_SYSTEMINFO
#		ifdef LOG_HAVE_SYS_UTSNAME_H
#			include <sys/utsname.h>
#		endif //LOG_HAVE_SYS_UTSNAME_H

#		ifdef LOG_HAVE_PWD_H
#			include <pwd.h>
#		endif //LOG_HAVE_PWD_H

#		ifdef LOG_HAVE_SYS_SYSINFO_H
#			include <sys/sysinfo.h>
#		endif //LOG_HAVE_SYS_SYSINFO_H
#   endif //LOG_USE_SYSTEMINFO

#endif //LOG_PLATFORM_WINDOWS


#ifdef LOG_PLATFORM_WINDOWS

#	if LOG_USE_MODULEDEFINITION
#		include <psapi.h>
#		pragma comment(lib,"psapi.lib")
#		pragma comment(lib,"version.lib")
#	endif //LOG_USE_MODULEDEFINITION

#	if INCLUDE_DBGHELP
#		include <dbghelp.h>
#		pragma comment(lib,"dbghelp.lib")
#	endif //INCLUDE_DBGHELP

#endif //LOG_PLATFORM_WINDOWS

#define LOG_SET_VERBOSE_LEVEL(l)  logging::configurator.set_verbose_level(l)

#if LOG_CONFIGURE_FROM_REGISTRY
#	define LOG_SET_REG_CONFIG_PATH(p)  logging::configurator.set_reg_config_path(p)
#else //LOG_CONFIGURE_FROM_REGISTRY
#	define LOG_SET_REG_CONFIG_PATH(p)
#endif //LOG_CONFIGURE_FROM_REGISTRY

#if LOG_INI_CONFIGURATION
#	define LOG_SET_INI_CONFIG_PATHS(p)  logging::configurator.set_ini_file_find_paths(p)
#else //LOG_INI_CONFIGURATION
#	define LOG_SET_INI_CONFIG_PATHS(p)
#endif //LOG_INI_CONFIGURATION

#if !defined(LOG_PLATFORM_WINDOWS) && LOG_USE_MODULEDEFINITION
#   include <dlfcn.h>
#   include <link.h>
#endif //!defined(LOG_PLATFORM_WINDOWS) && LOG_USE_MODULEDEFINITION

#if !defined(LOG_PLATFORM_WINDOWS) && LOG_AUTO_DEBUGGING
#   include <execinfo.h>
#   include <cxxabi.h>
#endif //!defined(LOG_PLATFORM_WINDOWS) && LOG_AUTO_DEBUGGING

#if !defined(LOG_PLATFORM_WINDOWS) && LOG_UNHANDLED_EXCEPTIONS
#   include <signal.h>
#   include <ucontext.h>
#endif //!defined(LOG_PLATFORM_WINDOWS) && LOG_UNHANDLED_EXCEPTIONS


#	define LOG_INFO(...) logging::_logger->log(logging::logger_verbose_info, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,__VA_ARGS__)
#	define LOG_DEBUG(...) logging::_logger->log(logging::logger_verbose_debug, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,__VA_ARGS__)
#	define LOG_WARNING(...) logging::_logger->log(logging::logger_verbose_warning, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,__VA_ARGS__)
#	define LOG_ERROR(...) logging::_logger->log(logging::logger_verbose_error, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,__VA_ARGS__)
#	define LOG_FATAL(...) logging::_logger->log(logging::logger_verbose_fatal, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,__VA_ARGS__)

#	define LOG_BINARY_INFO(p,stack_frame) logging::_logger->log_binary(logging::logger_verbose_info, \
		LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,p,stack_frame)
#	define LOG_BINARY_DEBUG(p,stack_frame) logging::_logger->log_binary(logging::logger_verbose_debug, \
		LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,p,stack_frame)
#	define LOG_BINARY_WARNING(p,stack_frame) logging::_logger->log_binary(logging::logger_verbose_warning, \
		LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,p,stack_frame)
#	define LOG_BINARY_ERROR(p,stack_frame) logging::_logger->log_binary(logging::logger_verbose_error, \
		LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,p,stack_frame)
#	define LOG_BINARY_FATAL(p,stack_frame) logging::_logger->log_binary(logging::logger_verbose_fatal, \
		LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,p,stack_frame)

#	define LOG_EXCEPTION_DEBUG(e) logging::_logger->log_exception(logging::logger_verbose_debug, \
		LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__, e)
#	define LOG_EXCEPTION_WARNING(e) logging::_logger->log_exception(logging::logger_verbose_warning, \
		LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__, e)
#	define LOG_EXCEPTION_INFO(e) logging::_logger->log_exception(logging::logger_verbose_info, \
		LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__, e)
#	define LOG_EXCEPTION_ERROR(e) logging::_logger->log_exception(logging::logger_verbose_error, \
		LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__, e)
#	define LOG_EXCEPTION_FATAL(e) logging::_logger->log_exception(logging::logger_verbose_fatal, \
		LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__, e)

#if LOG_USE_MODULEDEFINITION

#	define LOG_MODULES_DEBUG logging::_logger->log_modules(logging::logger_verbose_debug, \
	LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__)
#	define LOG_MODULES_INFO logging::_logger->log_modules(logging::logger_verbose_info, \
	LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__)
#	define LOG_MODULES_WARNING logging::_logger->log_modules(logging::logger_verbose_warning, \
	LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__)
#	define LOG_MODULES_ERROR logging::_logger->log_modules(logging::logger_verbose_error, \
	LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__)
#	define LOG_MODULES_FATAL logging::_logger->log_modules(logging::logger_verbose_fatal, \
	LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__)

#else //LOG_USE_MODULEDEFINITION

#	define LOG_MODULES_DEBUG 
#	define LOG_MODULES_INFO 
#	define LOG_MODULES_WARNING 
#	define LOG_MODULES_ERROR 
#	define LOG_MODULES_FATAL 

#endif //LOG_USE_MODULEDEFINITION

#if LOG_AUTO_DEBUGGING

#	define LOG_STACKTRACE_DEBUG logging::_logger->log_stack_trace(logging::logger_verbose_debug, \
		LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__)
#	define LOG_STACKTRACE_INFO logging::_logger->log_stack_trace(logging::logger_verbose_info, \
		LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__)
#	define LOG_STACKTRACE_WARNING logging::_logger->log_stack_trace(logging::logger_verbose_warning, \
		LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__)
#	define LOG_STACKTRACE_ERROR logging::_logger->log_stack_trace(logging::logger_verbose_error, \
		LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__)
#	define LOG_STACKTRACE_FATAL logging::_logger->log_stack_trace(logging::logger_verbose_fatal, \
		LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__)

#else //LOG_AUTO_DEBUGGING

#	define LOG_STACKTRACE_DEBUG 
#	define LOG_STACKTRACE_INFO 
#	define LOG_STACKTRACE_WARNING
#	define LOG_STACKTRACE_ERROR
#	define LOG_STACKTRACE_FATAL

#endif //LOG_AUTO_DEBUGGING

#if LOG_UNHANDLED_EXCEPTIONS && defined(LOG_PLATFORM_WINDOWS)
#	define  DEFINE_LOG_UNHANDLED_EXCEPTIONS_MEMBERS    LPTOP_LEVEL_EXCEPTION_FILTER logging::unhandled_exceptions_processor::prev_exception_filter = NULL;
#else //LOG_UNHANDLED_EXCEPTIONS && defined(LOG_PLATFORM_WINDOWS)
#	define  DEFINE_LOG_UNHANDLED_EXCEPTIONS_MEMBERS
#endif //LOG_UNHANDLED_EXCEPTIONS && defined(LOG_PLATFORM_WINDOWS)


#if LOG_SHARED
#	define DEFINE_LOGGER   logging::log_configurator logging::configurator; \
	logging::singleton<logging::logger_interface, logging::logger> logging::_logger ( \
	&logging::logger_interface::ref, &logging::logger_interface::deref, &logging::logger_interface::ref_counter, \
	(logging::logger_interface*)logging::shared_obj::try_found_shared_object(0), false); \
							DEFINE_LOG_UNHANDLED_EXCEPTIONS_MEMBERS
#else //LOG_SHARED
#	define DEFINE_LOGGER   logging::log_configurator logging::configurator; \
							logging::singleton<logging::logger_interface, logging::logger> logging::_logger; \
							DEFINE_LOG_UNHANDLED_EXCEPTIONS_MEMBERS
#endif //LOG_SHARED


#if LOG_INI_CONFIGURATION

// simple INI parser
namespace logging_ini {

/* inih -- simple .INI file parser
inih is released under the New BSD license (see LICENSE.txt). Go to the project
home page for more info:
http://code.google.com/p/inih/
*/
/* Nonzero to allow multi-line value parsing, in the style of Python's
   ConfigParser. If allowed, ini_parse() will call the handler with the same
   name for each subsequent line parsed. */
#ifndef INI_ALLOW_MULTILINE
#define INI_ALLOW_MULTILINE 1
#endif

/* Nonzero to allow a UTF-8 BOM sequence (0xEF 0xBB 0xBF) at the start of
   the file. See http://code.google.com/p/inih/issues/detail?id=21 */
#ifndef INI_ALLOW_BOM
#define INI_ALLOW_BOM 1
#endif

/* Stop parsing on first error (default is to keep parsing). */
#ifndef INI_STOP_ON_FIRST_ERROR
#define INI_STOP_ON_FIRST_ERROR 0
#endif

/* Maximum line length for any line in INI file. */
#ifndef INI_MAX_LINE
#define INI_MAX_LINE 8192
#endif


#define MAX_SECTION 256
#define MAX_NAME 256

extern "C" {

/* Strip whitespace chars off end of given string, in place. Return s. */
static char* rstrip(char* s)
{
    char* p = s + strlen(s);
    while (p > s && isspace((unsigned char)(*--p)))
        *p = '\0';
    return s;
}

/* Return pointer to first non-whitespace char in given string. */
static char* lskip(const char* s)
{
    while (*s && isspace((unsigned char)(*s)))
        s++;
    return (char*)s;
}

/* Return pointer to first char c or ';' comment in given string, or pointer to
   null at end of string if neither found. ';' must be prefixed by a whitespace
   character to register as a comment. */
static char* find_char_or_comment(const char* s, char c)
{
    int was_whitespace = 0;
    while (*s && *s != c && !(was_whitespace /*&& *s == ';'*/)) {
        was_whitespace = isspace((unsigned char)(*s));
        s++;
    }
    return (char*)s;
}

/* Version of strncpy that ensures dest (size bytes) is null-terminated. */
static char* strncpy0(char* dest, const char* src, size_t size)
{
    strncpy(dest, src, size);
    dest[size - 1] = '\0';
    return dest;
}

/* See documentation in header file. */
static int ini_parse_file(FILE* file,
                   int (*handler)(void*, const char*, const char*,
                                  const char*),
                   void* user)
{
    /* Uses a fair bit of stack (use heap instead if you need to) */
    char* line;

    char section[MAX_SECTION] = "";
    char prev_name[MAX_NAME] = "";

    char* start;
    char* end;
    char* name;
    char* value;
    int lineno = 0;
    int error = 0;

    line = (char*)malloc(INI_MAX_LINE);
    if (!line) {
        return -2;
    }

    /* Scan through file line by line */
    while (fgets(line, INI_MAX_LINE, file) != NULL) {
        lineno++;

        start = line;
#if INI_ALLOW_BOM
        if (lineno == 1 && (unsigned char)start[0] == 0xEF &&
                           (unsigned char)start[1] == 0xBB &&
                           (unsigned char)start[2] == 0xBF) {
            start += 3;
        }
#endif
        start = lskip(rstrip(start));

        if (*start == ';' || *start == '#') {
            /* Per Python ConfigParser, allow '#' comments at start of line */
        }
#if INI_ALLOW_MULTILINE
        else if (*prev_name && *start && start > line) {
            /* Non-black line with leading whitespace, treat as continuation
               of previous name's value (as per Python ConfigParser). */
            if (!handler(user, section, prev_name, start) && !error)
                error = lineno;
        }
#endif
        else if (*start == '[') {
            /* A "[section]" line */
            end = find_char_or_comment(start + 1, ']');
            if (*end == ']') {
                *end = '\0';
                strncpy0(section, start + 1, sizeof(section));
                *prev_name = '\0';
            }
            else if (!error) {
                /* No ']' found on section line */
                error = lineno;
            }
        }
        else if (*start && *start != ';') {
            /* Not a comment, must be a name[=:]value pair */
            end = find_char_or_comment(start, '=');
            if (*end != '=') {
                end = find_char_or_comment(start, ':');
            }
            if (*end == '=' || *end == ':') {
                *end = '\0';
                name = rstrip(start);
                value = lskip(end + 1);
                end = find_char_or_comment(value, '\0');
                if (*end == ';')
                    *end = '\0';
                rstrip(value);

                /* Valid name[=:]value pair found, call handler */
                strncpy0(prev_name, name, sizeof(prev_name));
                if (!handler(user, section, name, value) && !error)
                    error = lineno;
            }
            else if (!error) {
                /* No '=' or ':' found on name[=:]value line */
                error = lineno;
            }
        }

#if INI_STOP_ON_FIRST_ERROR
        if (error)
            break;
#endif
    }

    free(line);

    return error;
}

/* See documentation in header file. */
static int ini_parse(const char* filename,
              int (*handler)(void*, const char*, const char*, const char*),
              void* user)
{
    FILE* file;
    int error;

    file = fopen(filename, "r");
    if (!file)
        return -1;
    error = ini_parse_file(file, handler, user);
    fclose(file);
    return error;
}

} //extern "C"

}// namespace logging_ini

#endif //LOG_INI_CONFIGURATION


namespace logging 
{
typedef unsigned long uint32_t;

#if defined(LOG_COMPILER_GCC) || defined(LOG_COMPILER_MINGW) || defined(LOG_COMPILER_ICC) || defined(LOG_COMPILER_SOLARIS)
typedef unsigned long long uint64_t;
#endif //defined(LOG_COMPILER_GCC) || defined(LOG_COMPILER_MINGW) || defined(LOG_COMPILER_ICC) || defined(LOG_COMPILER_SOLARIS)

#ifdef LOG_COMPILER_MSVC
typedef unsigned __int64 uint64_t;
typedef __int64 int64_t;
#endif //LOG_COMPILER_MSVC

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef long int32_t;
typedef short int16_t;
typedef char int8_t;

#if defined(_AMD64_) || defined(__LP64__) || defined(_LP64)
    typedef uint64_t intptr_t;
#	define platform_dword_t DWORD64

#   define LOG_PLATFORM_64BIT

#else // defined(_AMD64_) || defined(__LP64__) || defined(_LP64)
	typedef unsigned int intptr_t;
#	define platform_dword_t DWORD

#   define LOG_PLATFORM_32BIT

#endif // defined(_AMD64_) || defined(__LP64__) || defined(_LP64)


////////////////////    Helpers     ////////////////////

template<typename _TIf, 
		typename _TImpl = _TIf>
class singleton
{
public:
	singleton(void(_TIf::*ref_fn)() = NULL, 
				void(_TIf::*deref_fn)() = NULL, 
				int(_TIf::*ref_cnt_fn)() = NULL,
				_TIf* ptr = NULL,
				bool need_delete = false)
		:ptr_(NULL), need_delete_(false), ref_fn_(ref_fn), deref_fn_(deref_fn), ref_cnt_fn_(ref_cnt_fn)
	{
		if(ptr)
			reset(ptr, need_delete);
	}

	~singleton()
	{
		release();
	}

	__inline _TIf* operator ->() { return get(); }

	_TIf* get()
	{
		if (!ptr_)
		{
			ptr_ = new _TImpl();
			need_delete_ = true;

			if (ref_fn_)
				(ptr_->*ref_fn_)();
		}

		return ptr_;
	}

	void reset(_TIf* ptr, bool need_delete = true)
	{
		release();

		need_delete_ = need_delete;
		ptr_ = ptr;

		if (ref_fn_)
			(ptr_->*ref_fn_)();
	}

	void release()
	{
		if (deref_fn_ && ptr_)
			(ptr_->*deref_fn_)();
				
		if (ref_cnt_fn_ && ptr_)
		{
			if ((ptr_->*ref_cnt_fn_)())
				ptr_ = NULL;
		}

		if (ptr_ && need_delete_) delete ptr_;
		ptr_ = NULL;
		need_delete_ = false;
	}

private:
    _TIf* ptr_;
    bool need_delete_;

    void(_TIf::*ref_fn_)();
	void(_TIf::*deref_fn_)();
	int(_TIf::*ref_cnt_fn_)();
};

//////////////   String functions    //////////////

static void format_arguments_list(std::string& result, const char* format, va_list arguments)
{
	static const int StartBufferSize = 512;
	static const int BufferSizeIncrement = 512;

	char staticBuf[StartBufferSize];
	int bufferSize = StartBufferSize ;

	char* buffer = staticBuf;
	int need_delete = 0;

	do 
	{
#		ifdef LOG_COMPILER_MSVC
		int result = _vsnprintf_s(buffer, bufferSize, bufferSize-1, format, arguments);
#		else //LOG_COMPILER_MSVC
		int result = vsnprintf(buffer,bufferSize-1,format,arguments);
#		endif //LOG_COMPILER_MSVC

		if (result >= 0)
			break;

		bufferSize += BufferSizeIncrement;
		if (!need_delete)
		{
			need_delete = 1;
			buffer = NULL;
		}

		buffer = (char*) realloc(buffer, bufferSize);
		if (!buffer)
			break;

	} while(true);
	
	result = buffer ? buffer : std::string();
	if (need_delete)
		free(buffer);
}

static std::string stringformat (const char* format, ...)
{
	va_list arguments;
	va_start(arguments, format);
	std::string result;
	format_arguments_list(result, format, arguments);
	va_end(arguments);
	return result;
}

static bool contains(const char* str, const char* find_str)
{
	return strstr(str,find_str) != NULL;
}

static std::string& replace(std::string& str,const char* find_str, const std::string& replaceStr)
{
	size_t i=0;
	const char* ptr = NULL;
	size_t find_str_len = strlen(find_str);

	while (((ptr = strstr(str.c_str(),find_str)) != NULL) && ptr >= str.c_str())
	{
		str = static_cast<std::string>(str).replace(ptr-str.c_str(),find_str_len,replaceStr);
		i+=replaceStr.length();
	}
	return str;
}

static void split(std::string str, std::vector<std::string>& out_strings, char ch = ' ')
{
	while (true)
	{
		size_t position = str.find_first_of(ch);
		std::string current = str.substr(0,position);

		if (current.size())
			out_strings.push_back(current);

		if (position == std::string::npos)
			break;

		str = str.substr(position);
		position = str.find_first_not_of(ch);
		if (position == std::string::npos)
			break;

		str = str.substr(position);
	}
}


////////////////////  Module Definition components  ////////////////////

#if LOG_USE_MODULEDEFINITION

#ifdef LOG_PLATFORM_WINDOWS

class pe_image
{
public:
	enum pe_signatures
	{
		PE_IMAGE_DOS_SIGNATURE                = 0x5A4D,      // MZ
		PE_IMAGE_NT_SIGNATURE                 = 0x00004550  // PE00
	};

	enum pe_image_directory_entry_types
	{
		PE_IMAGE_DIRECTORY_ENTRY_EXPORT          = 0,   // Export Directory
		PE_IMAGE_DIRECTORY_ENTRY_IMPORT          = 1,   // Import Directory
		PE_IMAGE_DIRECTORY_ENTRY_RESOURCE        = 2,   // Resource Directory
		PE_IMAGE_DIRECTORY_ENTRY_EXCEPTION       = 3,   // Exception Directory
		PE_IMAGE_DIRECTORY_ENTRY_SECURITY        = 4,   // Security Directory
		PE_IMAGE_DIRECTORY_ENTRY_BASERELOC       = 5,   // Base Relocation Table
		PE_IMAGE_DIRECTORY_ENTRY_DEBUG           = 6,   // Debug Directory
		PE_IMAGE_DIRECTORY_ENTRY_ARCHITECTURE    = 7,   // Architecture Specific Data
		PE_IMAGE_DIRECTORY_ENTRY_GLOBALPTR       = 8,   // RVA of GP
		PE_IMAGE_DIRECTORY_ENTRY_TLS             = 9,   // TLS Directory
		PE_IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG     = 10,  // Load Configuration Directory
		PE_IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT    = 11,   // Bound Import Directory in headers
		PE_IMAGE_DIRECTORY_ENTRY_IAT             = 12,   // Import Address Table
		PE_IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT    = 13,   // Delay Load Import Descriptors
		PE_IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR  = 14   // COM Runtime descriptor
	};

	static const int PE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES   = 16;

	typedef struct _pe_image_dos_header_t 
	{      // DOS .EXE header
		unsigned short   e_magic;                     // Magic number
		unsigned short   e_cblp;                      // Bytes on last page of file
		unsigned short   e_cp;                        // Pages in file
		unsigned short   e_crlc;                      // Relocations
		unsigned short   e_cparhdr;                   // Size of header in paragraphs
		unsigned short   e_minalloc;                  // Minimum extra paragraphs needed
		unsigned short   e_maxalloc;                  // Maximum extra paragraphs needed
		unsigned short   e_ss;                        // Initial (relative) SS value
		unsigned short   e_sp;                        // Initial SP value
		unsigned short   e_csum;                      // Checksum
		unsigned short   e_ip;                        // Initial IP value
		unsigned short   e_cs;                        // Initial (relative) CS value
		unsigned short   e_lfarlc;                    // File address of relocation table
		unsigned short   e_ovno;                      // Overlay number
		unsigned short   e_res[4];                    // Reserved words
		unsigned short   e_oemid;                     // OEM identifier (for e_oeminfo)
		unsigned short   e_oeminfo;                   // OEM information; e_oemid specific
		unsigned short   e_res2[10];                  // Reserved words
		unsigned long    e_lfanew;                    // File address of new exe header
	} pe_image_dos_header_t;

	typedef struct _pe_image_file_header_t {
		unsigned short Machine;
		unsigned short NumberOfSections;
		unsigned long TimeDateStamp;
		unsigned long PointerToSymbolTable;
		unsigned long NumberOfSymbols;
		unsigned short SizeOfOptionalHeader;
		unsigned short Characteristics;
	} pe_image_file_header_t;

	typedef struct _pe_image_data_directory_t 
	{
		unsigned long   VirtualAddress;
		unsigned long   Size;
	} pe_image_data_directory_t;

	typedef struct _pe_image_optional_header64_t {
		unsigned short        Magic;
		unsigned char        MajorLinkerVersion;
		unsigned char        MinorLinkerVersion;
		unsigned long       SizeOfCode;
		unsigned long       SizeOfInitializedData;
		unsigned long       SizeOfUninitializedData;
		unsigned long       AddressOfEntryPoint;
		unsigned long       BaseOfCode;
		uint64_t   ImageBase;
		unsigned long       SectionAlignment;
		unsigned long       FileAlignment;
		unsigned short        MajorOperatingSystemVersion;
		unsigned short        MinorOperatingSystemVersion;
		unsigned short        MajorImageVersion;
		unsigned short        MinorImageVersion;
		unsigned short        MajorSubsystemVersion;
		unsigned short        MinorSubsystemVersion;
		unsigned long       Win32VersionValue;
		unsigned long       SizeOfImage;
		unsigned long       SizeOfHeaders;
		unsigned long       CheckSum;
		unsigned short        Subsystem;
		unsigned short        DllCharacteristics;
		uint64_t   SizeOfStackReserve;
		uint64_t   SizeOfStackCommit;
		uint64_t   SizeOfHeapReserve;
		uint64_t   SizeOfHeapCommit;
		unsigned long       LoaderFlags;
		unsigned long       NumberOfRvaAndSizes;
		pe_image_data_directory_t DataDirectory[PE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
	} pe_image_optional_header64_t;

	typedef struct _pe_image_optional_header32_t 
	{
		unsigned short    Magic;
		unsigned char    MajorLinkerVersion;
		unsigned char    MinorLinkerVersion;
		unsigned long   SizeOfCode;
		unsigned long   SizeOfInitializedData;
		unsigned long   SizeOfUninitializedData;
		unsigned long   AddressOfEntryPoint;
		unsigned long   BaseOfCode;
		unsigned long   BaseOfData;
		unsigned long   ImageBase;
		unsigned long   SectionAlignment;
		unsigned long   FileAlignment;
		unsigned short    MajorOperatingSystemVersion;
		unsigned short    MinorOperatingSystemVersion;
		unsigned short    MajorImageVersion;
		unsigned short    MinorImageVersion;
		unsigned short    MajorSubsystemVersion;
		unsigned short    MinorSubsystemVersion;
		unsigned long   Win32VersionValue;
		unsigned long   SizeOfImage;
		unsigned long   SizeOfHeaders;
		unsigned long   CheckSum;
		unsigned short    Subsystem;
		unsigned short    DllCharacteristics;
		unsigned long   SizeOfStackReserve;
		unsigned long   SizeOfStackCommit;
		unsigned long   SizeOfHeapReserve;
		unsigned long   SizeOfHeapCommit;
		unsigned long   LoaderFlags;
		unsigned long   NumberOfRvaAndSizes;
		pe_image_data_directory_t DataDirectory[PE_IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
	} pe_image_optional_header32_t;

	typedef struct _pe_image_nt_headers64_t
	{
		unsigned long Signature;
		pe_image_file_header_t FileHeader;
		pe_image_optional_header64_t OptionalHeader;
	} pe_image_nt_headers64_t;

	typedef struct _pe_image_nt_headers32_t {
		unsigned long Signature;
		pe_image_file_header_t FileHeader;
		pe_image_optional_header32_t OptionalHeader;
	} pe_image_nt_headers32_t;
	

#ifdef _WIN64
	typedef pe_image_nt_headers64_t pe_image_nt_headers_t;
#else
	typedef pe_image_nt_headers32_t pe_image_nt_headers_t;
#endif

public:
    static void* api_find_module_entry_point(intptr_t some_addr_in_module)
	{
#if !LOG_USE_SEH
		HMODULE h;
		if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)(void*)(some_addr_in_module), &h))
			return reinterpret_cast<void*>(h);

		return NULL;
#else //LOG_USE_SEH
		const int max_cycles_find = 0x4000;
		const int max_exceptions_per_try = 0x100;
		
		int cycles = 0;
		int exceptions = 0;

#ifdef _AMD64_
		some_addr_in_module &= 0xFFFFFFFFFFFF0000;
#else //_AMD64_
		some_addr_in_module &= 0xFFFF0000;
#endif //_AMD64_

		while(1)
		{
			const pe_image_dos_header_t* doshdr = (pe_image_dos_header_t*)some_addr_in_module;

			__try
			{
				if (doshdr->e_magic == PE_IMAGE_DOS_SIGNATURE)
				{
					if (doshdr->e_lfanew < 0x100000)
					{
						const pe_image_nt_headers_t* nthdr = (pe_image_nt_headers_t*)(some_addr_in_module + doshdr->e_lfanew);
						if (nthdr->Signature == PE_IMAGE_NT_SIGNATURE)
							break;
					}
				}

				exceptions = 0;
			}
			__except(1)
			{
				++exceptions;
			}

			++cycles;
			some_addr_in_module -= 0x1000;

			if (cycles > max_cycles_find)
				return NULL;

			if (exceptions > max_exceptions_per_try)
				return NULL;
		}

		return (void*)some_addr_in_module;
#endif //LOG_USE_SEH
	}
};
#endif //LOG_PLATFORM_WINDOWS


struct module_entry_t
{
	std::string imageName;
	std::string moduleName;
	intptr_t baseAddress;
	unsigned long size;
	std::string file_version;
	std::string product_version;
	std::string file_description;
	std::string company_name;
	unsigned long lang_id;

	module_entry_t() : baseAddress(0), size(0), lang_id(0) {}
}; 

class module_definition
{
public:
	static std::string module_name_by_addr(void* functionAddress)
	{
#ifdef LOG_PLATFORM_WINDOWS
		HMODULE module = reinterpret_cast<HMODULE>(pe_image::api_find_module_entry_point(
			reinterpret_cast<intptr_t>(functionAddress)));
		
		if (!module)
			return std::string();

		char fileName[MAX_PATH];
		GetModuleFileNameA(module,fileName,sizeof(fileName));
		return fileName;
#else //LOG_PLATFORM_WINDOWS
        Dl_info info;
        if (dladdr(functionAddress, &info))
        {
            return info.dli_fname;
        }

        return "";
#endif //LOG_PLATFORM_WINDOWS
	}

#ifdef LOG_PLATFORM_WINDOWS
	static bool module_query_version_info(HMODULE module, std::string& file_version, 
					std::string& file_description, std::string& product_version, 
					std::string& company_name,
					unsigned long& lang_id)
	{
		char szFilename[MAX_PATH + 1];
		
		if (GetModuleFileNameA(module, szFilename, MAX_PATH) == 0)
		{
			return false;
		}

		// allocate a block of memory for the version info
		DWORD dummy;
		DWORD dwSize = GetFileVersionInfoSizeA(szFilename, &dummy);
		if (dwSize == 0)
		{
			return false;
		}

		uint8_t* data = (uint8_t*) malloc(dwSize);
		if (!data)
			return false;

		// load the version info
		if (!GetFileVersionInfoA(szFilename, 0, dwSize, data))
		{
			free(data);
			return false;
		}

		DWORD vLen,langD;
        BOOL ret_val;    
    
        LPVOID retbuf=NULL;
    
        static char file_entry[256];

        sprintf(file_entry,"\\VarFileInfo\\Translation");
        ret_val = VerQueryValueA(data,file_entry,&retbuf,(UINT *)&vLen);
        if (ret_val && vLen==4) 
        {
	        static char current_entry[256];

			memcpy(&langD,retbuf,4);            

			lang_id = langD;

			sprintf(file_entry, "\\StringFileInfo\\%02X%02X%02X%02X\\",
                  (langD & 0xff00)>>8,langD & 0xff,(langD & 0xff000000)>>24, 
                  (langD & 0xff0000)>>16);            

			strcpy(current_entry, file_entry);
			strcat(current_entry, "FileVersion");

			if (VerQueryValueA(data,current_entry,&retbuf,(UINT *)&vLen)) 
				file_version = (char*)retbuf;

			strcpy(current_entry, file_entry);
			strcat(current_entry, "FileDescription");

			if (VerQueryValueA(data,current_entry,&retbuf,(UINT *)&vLen)) 
				file_description = (char*)retbuf;

			strcpy(current_entry, file_entry);
			strcat(current_entry, "ProductVersion");

			if (VerQueryValueA(data,current_entry,&retbuf,(UINT *)&vLen)) 
				product_version = (char*)retbuf;

			strcpy(current_entry, file_entry);
			strcat(current_entry, "CompanyName");

			if (VerQueryValueA(data,current_entry,&retbuf,(UINT *)&vLen)) 
				company_name = (char*)retbuf;
        }

		free(data);

		return true;
	}

#endif //LOG_PLATFORM_WINDOWS


#if !defined(LOG_PLATFORM_WINDOWS)
    static int module_callback(struct dl_phdr_info *info, size_t size, void *data)
    {
        std::list<module_entry_t>* modules = (std::list<module_entry_t>*)data;

        if (!info || size < sizeof(struct dl_phdr_info))
            return 0;

        if (!strlen(info->dlpi_name))
            return 0;

        module_entry_t e;
        e.baseAddress = info->dlpi_addr;

        e.imageName = info->dlpi_name;
        e.moduleName = info->dlpi_name;

        size_t last_delim = e.moduleName.find_last_of('/');
        if (last_delim == std::string::npos)
            last_delim = 0;
        else
            last_delim++;

        e.moduleName = e.moduleName.substr(last_delim);

        e.size = 0;

        for (int j = 0; j < info->dlpi_phnum; j++)
        {
            uintptr_t ptr = info->dlpi_phdr[j].p_vaddr + info->dlpi_phdr[j].p_memsz; // - info->dlpi_addr;

            if (e.size < ptr)
                e.size = ptr;
        }

        modules->push_back(e);
        return 0;
    }
#endif


	static bool query_module_list(std::list<module_entry_t> &modules)
	{
#ifdef LOG_PLATFORM_WINDOWS
		const int MaxModules = 1024;
		modules.clear();

		HMODULE moduleArray[MaxModules];
		DWORD bytesNeeded;

		HANDLE processHandle = GetCurrentProcess(); 

		if (!EnumProcessModules(processHandle, moduleArray, MaxModules*sizeof(HMODULE), &bytesNeeded))
			return false;

		for (size_t i = 0; i < bytesNeeded / sizeof(HMODULE); i++ )
		{
			MODULEINFO moduleInfo;
			GetModuleInformation(processHandle, moduleArray[i], &moduleInfo, sizeof moduleInfo );

			module_entry_t e;
			e.baseAddress = reinterpret_cast<intptr_t>(moduleInfo.lpBaseOfDll);
			e.size = moduleInfo.SizeOfImage;

			char name[MAX_PATH];
			GetModuleFileNameExA(processHandle, moduleArray[i], name, sizeof(name));
			e.imageName = name;

			GetModuleBaseNameA(processHandle, moduleArray[i], name, sizeof(name));
			e.moduleName = name;

			module_query_version_info(reinterpret_cast<HMODULE>(moduleInfo.lpBaseOfDll), 
				e.file_version, e.file_description, e.product_version, e.company_name, e.lang_id);

			modules.push_back(e);
		}

#else //LOG_PLATFORM_WINDOWS
        dl_iterate_phdr(&module_callback, &modules);
#endif //LOG_PLATFORM_WINDOWS

		return true;
	}
};

#endif // LOG_USE_MODULEDEFINITION


//////////////////////////////////////////////////////////////

class utils
{
public:

    static struct tm get_time(int& millisec)
	{
        struct tm newtime;

#ifdef LOG_PLATFORM_WINDOWS

	#ifdef  LOG_COMPILER_MSVC
		struct __timeb64 tstruct;
		_ftime64(&tstruct);
		_localtime64_s(&newtime, &tstruct.time);
	#else //LOG_COMPILER_MSVC

		struct timeb tstruct;
		ftime(&tstruct);
		newtime = *localtime(&tstruct.time);
	#endif //LOG_COMPILER_MSVC

		millisec = tstruct.millitm;

#else //LOG_PLATFORM_WINDOWS
        time_t tim = time(NULL);
        struct tm *loc_time = localtime(&tim);

        memcpy(&newtime, loc_time,sizeof(struct tm));

        struct timeval tv;
        gettimeofday(&tv,NULL);
        millisec = tv.tv_usec / 1000;
#endif //LOG_PLATFORM_WINDOWS

		return newtime;
	}


#ifndef _WIN32

    static std::string do_readlink(std::string const& path)
    {
        char buff[1024];

#ifdef LOG_HAVE_UNISTD_H
        ssize_t len = ::readlink(path.c_str(), buff, sizeof(buff)-1);
#else //LOG_HAVE_UNISTD_H
	ssize_t len = -1;
#endif //LOG_HAVE_UNISTD_H

        if (len != -1)
        {
            buff[len] = '\0';
            return std::string(buff);
        }

        return std::string();
    }

#endif //_WIN32

#if defined(LOG_PLATFORM_LINUX) || defined(LOG_PLATFORM_CYGWIN)
#   define LOG_SELF_PROC_LINK "/proc/self/exe"
#endif //defined(LOG_PLATFORM_LINUX) || defined(LOG_PLATFORM_CYGWIN)

#ifdef LOG_PLATFORM_UNIX
#   define LOG_SELF_PROC_LINK "/proc/curproc/file"
#endif //LOG_PLATFORM_UNIX

#ifdef LOG_PLATFORM_SOLARIS
#   define LOG_SELF_PROC_LINK "/proc/self/path/a.out"
#endif //LOG_PLATFORM_SOLARIS


	static std::string get_process_file_path()
	{
#ifdef LOG_PLATFORM_WINDOWS
        char fileName[MAX_PATH];

		GetModuleFileNameA(NULL,fileName,sizeof(fileName));
		std::string path = fileName;

		size_t lastDelimiter = path.find_last_of("\\");
		return path.substr(0,lastDelimiter);
#else //LOG_PLATFORM_WINDOWS

        std::string path = do_readlink(LOG_SELF_PROC_LINK);
        return path.substr(0,path.find_last_of('/'));

#endif //LOG_PLATFORM_WINDOWS
	}

	static std::string get_process_full_file_name()
	{
#ifdef LOG_PLATFORM_WINDOWS
        char file_name[MAX_PATH];

		GetModuleFileNameA(NULL,file_name,sizeof(file_name));
		std::string path = file_name;

		size_t last_delim = path.find_last_of("\\");
		if (last_delim != std::string::npos)
			last_delim++;

		return path.substr(last_delim,std::string::npos);
#else //LOG_PLATFORM_WINDOWS
        std::string path = do_readlink(LOG_SELF_PROC_LINK);

        size_t last_delim = path.find_last_of("/");
        if (last_delim != std::string::npos)
            last_delim++;
        else
	return path;

        return path.substr(last_delim,std::string::npos);
#endif //LOG_PLATFORM_WINDOWS
    }

	static std::string get_process_file_name()
	{
		std::string full_name = get_process_full_file_name();

        size_t start = full_name.find_last_of('/');
        if (start != std::string::npos)
            start++;
        else
            start = 0;

		size_t count = full_name.find_last_of(".");
        return full_name.substr(start,count);
	}
};

//////////////////////////////////////////////////////////////
#if LOG_UNHANDLED_EXCEPTIONS
    static void init_unhandled_exceptions_handler();
#endif //LOG_UNHANDLED_EXCEPTIONS

//////////////////////////////////////////////////////////////

#if (LOG_CONFIGURE_FROM_REGISTRY || LOG_USE_SYSTEMINFO) && defined(LOG_PLATFORM_WINDOWS)
#	define LOG_DEFINE_REGISTRY_HELPER
#endif //(LOG_CONFIGURE_FROM_REGISTRY || LOG_USE_SYSTEMINFO) && defined(LOG_PLATFORM_WINDOWS)

#ifdef LOG_DEFINE_REGISTRY_HELPER
struct log_registry_helper
{
	static HKEY reg_base_key_by_string(const std::string& storage)
	{
		if (storage == "HKLM" || storage == "HKEY_LOCAL_MACHINE")
			return HKEY_LOCAL_MACHINE;

		if (storage == "HKCU" || storage == "HKEY_CURRENT_USER")
			return HKEY_CURRENT_USER;

		if (storage == "HKCR" || storage == "HKEY_CLASSES_ROOT")
			return HKEY_CLASSES_ROOT;

		if (storage == "HKU" || storage == "HKEY_USERS")
			return HKEY_USERS;

		return HKEY_CURRENT_USER;
	}

	static bool get_reg_dword_value(HKEY baseKey, const std::string& regPath, 
		const std::string& valueName, unsigned long& value)
	{
		HKEY   hkey ;
		LONG ret = RegOpenKeyExA(baseKey, regPath.c_str(), 0, KEY_QUERY_VALUE, &hkey);
		if (ret != ERROR_SUCCESS)
			return false;

		DWORD size = 0;
		DWORD type;
		ret = -1;
		RegQueryValueExA(hkey,valueName.c_str(),0,&type,NULL,&size);

		if (type == REG_DWORD)
		{
			size = sizeof(long);
			ret = RegQueryValueExA(hkey,valueName.c_str(),0,&type,(BYTE*)&value,&size);
		}

		RegCloseKey(hkey);
		return ret == ERROR_SUCCESS;
	}

	static bool get_reg_sz_value(HKEY baseKey, const std::string& regPath, 
		const std::string& valueName, std::string& result)
	{
		HKEY   hkey ;
		LONG ret = RegOpenKeyExA(baseKey, regPath.c_str(), 0, KEY_QUERY_VALUE, &hkey);
		if (ret != ERROR_SUCCESS)
			return false;

		DWORD size = 0;
		DWORD type;
		ret = -1;
		RegQueryValueExA(hkey,valueName.c_str(),0,&type,NULL,&size);

		if (type == REG_SZ)
		{
			size_t bufSize = 512;

			std::string value;
			value.resize(bufSize);

			DWORD size = static_cast<DWORD>(value.length())+1;
			ret = RegQueryValueExA(hkey,valueName.c_str(),0,&type,(BYTE*)value.data(),&size);

			while(ret == ERROR_MORE_DATA)
			{
				bufSize += 256;
				value.resize(bufSize);

				size = static_cast<DWORD>(value.length())+1;
				ret = RegQueryValueExA(hkey,valueName.c_str(),NULL,&type,(BYTE*)value.data(),&size);
			}

			if (ret == ERROR_SUCCESS)
				result = value.c_str();
		}

		RegCloseKey(hkey);
		return ret == ERROR_SUCCESS;
	}
};
#endif //LOG_DEFINE_REGISTRY_HELPER

//////////////////////////////////////////////////////////////
#if LOG_USE_SYSTEMINFO

	static std::string get_processor_arch()
	{
#ifdef LOG_PLATFORM_WINDOWS
        SYSTEM_INFO si;
		::GetSystemInfo(&si);

		std::string processorInfo;

		switch (si.wProcessorArchitecture)
		{
			case PROCESSOR_ARCHITECTURE_INTEL:
				processorInfo += "IA-32";
				break;
			case PROCESSOR_ARCHITECTURE_AMD64:
				processorInfo += "AMD64";
				break;
			case PROCESSOR_ARCHITECTURE_IA64:
				processorInfo += "IA64";
				break;
			case PROCESSOR_ARCHITECTURE_UNKNOWN:
			default:
				processorInfo += "Unknown";
		}
		processorInfo += stringformat(", count: %d", si.dwNumberOfProcessors);
		return processorInfo;
#else //LOG_PLATFORM_WINDOWS


#ifdef LOG_HAVE_SYS_UTSNAME_H
        struct utsname osname;
        uname(&osname);

        return osname.machine;
#else //LOG_HAVE_SYS_UTSNAME_H
	if (sizeof(int) == 4)
	    return "32-bit";

	if (sizeof(int) == 8)
	    return "64-bit";

	if (sizeof(int) == 2)
	    return "16-bit";

	if (sizeof(int) == 16)
	    return "128-bit";

	return std::string();
#endif //LOG_HAVE_SYS_UTSNAME_H

#endif //LOG_PLATFORM_WINDOWS
	}

#   ifdef LOG_PLATFORM_WINDOWS
    static bool is_wow64_used()
	{
		BOOL isWow;
		::IsWow64Process(::GetCurrentProcess(),&isWow);
		return isWow ? true : false;
	}

	typedef void (WINAPI *GetNativeSystemInfo_t)(LPSYSTEM_INFO);
	typedef BOOL (WINAPI *GetProductInfo_t)(DWORD, DWORD, DWORD, DWORD, PDWORD);


#   endif //LOG_PLATFORM_WINDOWS

	static bool get_os_version_display_string(std::string& os_version)
	{
#ifdef LOG_PLATFORM_WINDOWS

#ifndef VER_SUITE_WH_SERVER
#define VER_SUITE_WH_SERVER 0x00008000
#endif

#ifndef PRODUCT_PROFESSIONAL
#define PRODUCT_PROFESSIONAL 0x00000030
#endif

		OSVERSIONINFOEXA osvi;
		SYSTEM_INFO si;
		DWORD dwType;

		os_version = "";
		ZeroMemory(&si, sizeof(SYSTEM_INFO));
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEXA));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);

		if (!GetVersionExA((OSVERSIONINFOA*) &osvi))
			return false;

		GetNativeSystemInfo_t getNativeSystemInfoFn = reinterpret_cast<GetNativeSystemInfo_t>(
			GetProcAddress(GetModuleHandleA("kernel32.dll"), "GetNativeSystemInfo"));

		if (getNativeSystemInfoFn != NULL)
			getNativeSystemInfoFn(&si);
		else GetSystemInfo(&si);

		if (VER_PLATFORM_WIN32_NT!=osvi.dwPlatformId || osvi.dwMajorVersion <= 4)
			return false;

		os_version += "Microsoft ";

		if ( osvi.dwMajorVersion == 6 )
		{
			if( osvi.dwMinorVersion == 0 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					os_version += "Windows Vista ";
				else 
					os_version += "Windows Server 2008 ";
			}

			if ( osvi.dwMinorVersion == 1 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					os_version += "Windows 7 ";
				else 
					os_version += "Windows Server 2008 R2 ";
			}

			if ( osvi.dwMinorVersion == 2 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					os_version += "Windows 8 ";
				else 
					os_version += "Windows Server 2012 ";
			}


			if ( osvi.dwMinorVersion == 3 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					os_version += "Windows 8.1 ";
				else 
					os_version += "Windows Server 2012 R2 ";
			}

		 GetProductInfo_t pGPI = reinterpret_cast<GetProductInfo_t>(
			 GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo"));
		 pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

		 switch( dwType )
		 {
			case PRODUCT_ULTIMATE: os_version += "Ultimate Edition";  break;
			case PRODUCT_PROFESSIONAL: os_version += "Professional";  break;
			case PRODUCT_HOME_PREMIUM: os_version += "Home Premium Edition";  break;
			case PRODUCT_HOME_BASIC: os_version += "Home Basic Edition";  break;
			case PRODUCT_ENTERPRISE:  os_version += "Enterprise Edition";  break;
			case PRODUCT_BUSINESS: os_version += "Business Edition";  break;
			case PRODUCT_STARTER: os_version += "Starter Edition";  break;
			case PRODUCT_CLUSTER_SERVER:  os_version += "Cluster Server Edition";  break;
			case PRODUCT_DATACENTER_SERVER:  os_version += "Datacenter Edition";  break;
			case PRODUCT_DATACENTER_SERVER_CORE:  os_version += "Datacenter Edition (core installation)";  break;
			case PRODUCT_ENTERPRISE_SERVER:  os_version += "Enterprise Edition";  break;
			case PRODUCT_ENTERPRISE_SERVER_CORE: os_version += "Enterprise Edition (core installation)"; break;
			case PRODUCT_ENTERPRISE_SERVER_IA64: os_version += "Enterprise Edition for Itanium-based Systems"; break;
			case PRODUCT_SMALLBUSINESS_SERVER: os_version += "Small Business Server";  break;
			case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM: os_version += "Small Business Server Premium Edition"; break;
			case PRODUCT_STANDARD_SERVER: os_version += "Standard Edition"; break;
			case PRODUCT_STANDARD_SERVER_CORE:  os_version += "Standard Edition (core installation)";  break;
			case PRODUCT_WEB_SERVER: os_version += "Web Server Edition";  break;
		 }
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
		{
		 if( GetSystemMetrics(SM_SERVERR2) )
			os_version += "Windows Server 2003 R2, ";
		 else if ( osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER )
			os_version +=  "Windows Storage Server 2003";
		 else if ( osvi.wSuiteMask & VER_SUITE_WH_SERVER )
			os_version += "Windows Home Server";
		 else if( osvi.wProductType == VER_NT_WORKSTATION &&
				  si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
		 {
			os_version += "Windows XP Professional x64 Edition";
		 }
		 else os_version += "Windows Server 2003, ";

		 // Test for the server type.
		 if ( osvi.wProductType != VER_NT_WORKSTATION )
		 {
			if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
			{
				if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
				   os_version +=  "Datacenter Edition for Itanium-based Systems";
				else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
				   os_version += "Enterprise Edition for Itanium-based Systems";
			}

			else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
			{
				if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
				   os_version += "Datacenter x64 Edition";
				else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
				   os_version += "Enterprise x64 Edition";
				else os_version += "Standard x64 Edition";
			}
			else
			{
				if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
				   os_version += "Compute Cluster Edition";
				else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
				   os_version += "Datacenter Edition";
				else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
				   os_version += "Enterprise Edition";
				else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
				   os_version += "Web Edition";
				else os_version += "Standard Edition";
			}
		 }
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
		{
		 os_version += "Windows XP ";
		 if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
			os_version += "Home Edition";
		 else os_version += "Professional";
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
		{
		 os_version += "Windows 2000 ";

		 if ( osvi.wProductType == VER_NT_WORKSTATION )
		 {
			os_version += "Professional";
		 }
		 else 
		 {
			if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
			   os_version += "Datacenter Server";
			else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
			   os_version += "Advanced Server";
			else os_version += "Server";
		 }
		}

		if( strlen(osvi.szCSDVersion) > 0 )
		{
		  os_version += " ";
		  os_version += osvi.szCSDVersion;
		}

		os_version += stringformat(" (build %d)", osvi.dwBuildNumber);

		if ( osvi.dwMajorVersion >= 6 )
		{
		 if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
			os_version += ", 64-bit";
		 else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
			os_version += ", 32-bit";
		}

		return true; 
#else //LOG_PLATFORM_WINDOWS

#ifdef LOG_HAVE_SYS_UTSNAME_H
        struct utsname osname;
        uname(&osname);

        os_version = osname.sysname;
        os_version += " ";
        os_version += osname.release;
        os_version += " ";
        os_version += osname.version;

#else //LOG_HAVE_SYS_UTSNAME_H

	os_version = "Posix-based ";
#	ifdef LOG_PLATFORM_UNIX
	os_version += "Unix ";
#	endif //LOG_PLATFORM_UNIX

#	ifdef LOG_PLATFORM_BSD
	os_version += "BSD ";
#	endif //LOG_PLATFORM_BSD

#	ifdef LOG_PLATFORM_LINUX
	os_version += "Linux ";
#	endif //LOG_PLATFORM_LINUX

#	ifdef LOG_PLATFORM_CYGWIN
	os_version += "CYGWIN ";
#	endif //LOG_PLATFORM_CYGWIN

#	ifdef LOG_PLATFORM_FREEBSD
	os_version += "FreeBSD ";
#	endif //LOG_PLATFORM_FREEBSD

#	ifdef LOG_PLATFORM_NETBSD
	os_version += "NetBSD ";
#	endif //LOG_PLATFORM_NETBSD

#	ifdef LOG_PLATFORM_DRAGONFLYBSD
	os_version += "DragonFlyBSD";
#	endif //LOG_PLATFORM_DRAGONFLYBSD

#	ifdef LOG_PLATFORM_SOLARIS
	os_version += "Solaris ";
#	endif //LOG_PLATFORM_SOLARIS

#	ifdef LOG_PLATFORM_MAC
	os_version += "Mac-based ";
#	endif //LOG_PLATFORM_MAC

#	ifdef LOG_PLATFORM_MACOSX
	os_version += "MacOSX ";
#	endif //LOG_PLATFORM_MACOSX

#	ifdef LOG_PLATFORM_IPHONE
	os_version += "IPhone ";
#	endif //LOG_PLATFORM_IPHONE

#	ifdef LOG_PLATFORM_IPHONESIMULATOR
	os_version += "Simulator ";
#	endif //LOG_PLATFORM_IPHONESIMULATOR

#endif //LOG_HAVE_SYS_UTSNAME_H

        return true;
#endif //LOG_PLATFORM_WINDOWS
	}

#ifdef LOG_PLATFORM_WINDOWS
	static bool get_os_version_display_string2(std::string& os_version)
	{
		bool result;
		
		os_version = "";

		// Get version from registry
		std::string regProductName;
		std::string regEditionId;
		std::string regCurrentVersion;
		std::string regCurrentBuildNumber;

		result = log_registry_helper::get_reg_sz_value(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName", regProductName);
		result &= log_registry_helper::get_reg_sz_value(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentVersion", regCurrentVersion);
		result &= log_registry_helper::get_reg_sz_value(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentBuildNumber", regCurrentBuildNumber);
		log_registry_helper::get_reg_sz_value(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "EditionID", regEditionId);

		if (result)
			os_version = regProductName + std::string(" (") + regEditionId + std::string(") / Version ") + regCurrentVersion + std::string(" / Build ") + regCurrentBuildNumber;

		return result;
	}
#endif //LOG_PLATFORM_WINDOWS


	static bool get_host_name(std::string& name)
	{
	    name = "";

#ifdef LOG_PLATFORM_WINDOWS
        char buffer[MAX_PATH];
		DWORD size = sizeof(buffer);

		if (!::GetComputerNameExA(ComputerNamePhysicalDnsFullyQualified, buffer, &size))
		{
			if (!::GetComputerNameA(buffer, &size))
				return false;
		}

		name = buffer;
#else //LOG_PLATFORM_WINDOWS

#ifdef LOG_HAVE_SYS_UTSNAME_H
        struct utsname osname;
        uname(&osname);
        name = osname.nodename;
#endif //LOG_HAVE_SYS_UTSNAME_H

#endif //LOG_PLATFORM_WINDOWS
		return true;
	}


	static std::string get_memory_status()
	{
        std::string memoryString;

#ifdef LOG_PLATFORM_WINDOWS
		MEMORYSTATUSEX memoryStatus;
		memset(&memoryStatus,0,sizeof(MEMORYSTATUSEX));
		memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

		if (!GlobalMemoryStatusEx(&memoryStatus))
			return "";

        memoryString = stringformat("Total physical " LOG_FMT_I64 ", Total virtual " LOG_FMT_I64, memoryStatus.ullTotalPhys, memoryStatus.ullTotalVirtual);
		memoryString += stringformat("\nAvailable physical " LOG_FMT_I64 ", available virtual " LOG_FMT_I64 ", Load %d%%", memoryStatus.ullAvailPhys, memoryStatus.ullAvailVirtual, memoryStatus.dwMemoryLoad);
		memoryString += stringformat("\nPage file size: " LOG_FMT_I64 ", page memory available " LOG_FMT_I64, memoryStatus.ullTotalPageFile, memoryStatus.ullAvailPageFile);
#else //LOG_PLATFORM_WINDOWS

#ifdef LOG_HAVE_SYS_SYSINFO_H
        struct sysinfo info;
        sysinfo(&info);

        memoryString = stringformat("Total avail RAM %d, total swap %d, total high %d", info.totalram, info.totalswap, info.totalhigh);
        memoryString += stringformat("\nFree RAM %d, free swap %d, free high %d", info.freeram, info.freeswap, info.freehigh);
        memoryString += stringformat("\nMem used by buffers %d, shared RAM %d", info.bufferram, info.sharedram);
        memoryString += stringformat("\nMemory unit in bytes %d, process count: %d", info.mem_unit, info.procs);
        memoryString += stringformat("\nAvg load 1 min : 5 min : 15 min  --  %d:%d:%d", info.loads[0], info.loads[1], info.loads[2]);
#endif //LOG_HAVE_SYS_SYSINFO_H

#endif //LOG_PLATFORM_WINDOWS

		return memoryString;
	}

	static std::string get_current_user_name()
	{
#ifdef LOG_PLATFORM_WINDOWS
		char buf[MAX_PATH];
		unsigned long bufferSize = sizeof(buf);
		GetUserNameA(buf,&bufferSize);
		return buf;
#else //LOG_PLATFORM_WINDOWS

#ifdef LOG_HAVE_PWD_H
        struct passwd* pass;
        pass = getpwuid(getuid());

        if (!pass)
            return "(unknown)";

        return pass->pw_name;
#else //LOG_HAVE_PWD_H
	return "(cannot be obtained)";
#endif //LOG_HAVE_PWD_H

#endif //LOG_PLATFORM_WINDOWS
	}

	static std::string get_current_user_sid()
	{
#ifdef LOG_PLATFORM_WINDOWS
        HANDLE hToken;

		OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);

		DWORD len = 0;
		GetTokenInformation(hToken, TokenUser, 0, 0, &len);

		TOKEN_USER* pUser = (TOKEN_USER*)malloc(len);

		GetTokenInformation(hToken, TokenUser, pUser, len, &len);

		char* sid = 0;
		ConvertSidToStringSidA(pUser->User.Sid, &sid);
		
		std::string result = sid;

		CloseHandle(hToken);
		LocalFree(sid);
		free(pUser);

		return result;
#else // LOG_PLATFORM_WINDOWS

#ifdef LOG_HAVE_PWD_H
        std::string result = stringformat("%d", getuid());

        struct passwd* pass;
        pass = getpwuid(getuid());
        if (pass)
        {
            result += std::string(":");
            result += stringformat("%d",pass->pw_gid);
        }

        return result;
#else //LOG_HAVE_PWD_H
	return std::string();
#endif //LOG_HAVE_PWD_H

#endif // LOG_PLATFORM_WINDOWS
	}


	static std::string query_system_info()
	{
		std::stringstream system_info;
		system_info << "*********** SYSTEM INFORMATION *************" << std::endl;

		std::string os_info;
		if (get_os_version_display_string(os_info))
			system_info << "OS: " << os_info << std::endl;

#ifdef LOG_PLATFORM_WINDOWS
		if (get_os_version_display_string2(os_info))
			system_info << "OS from registry: " << os_info << std::endl;
#endif //LOG_PLATFORM_WINDOWS

		system_info << "Processor architecture: " << get_processor_arch() << std::endl;

#ifdef LOG_PLATFORM_WINDOWS
		if (is_wow64_used())
			system_info << "Used WOW64" << std::endl;
#endif //LOG_PLATFORM_WINDOWS

		std::string hostName;
		if (get_host_name(hostName))
			system_info << "Host Name: " << hostName << std::endl;

#ifdef LOG_PLATFORM_WINDOWS
        system_info << stringformat("Current tick count: %d",GetTickCount()) << std::endl;
#else //LOG_PLATFORM_WINDOWS

#	ifdef LOG_HAVE_SYS_SYSINFO_H
        struct sysinfo info;
        sysinfo(&info);

        system_info << stringformat("Uptime, seconds: %d", info.uptime) << std::endl;
#	endif //LOG_HAVE_SYS_SYSINFO_H

#endif //LOG_PLATFORM_WINDOWS

        system_info << "Current user SID: " << get_current_user_sid() << std::endl;
		system_info << "Current user name: " <<  get_current_user_name() << std::endl;

		system_info << "--- MEMORY ---" << std::endl;
		system_info << get_memory_status() << std::endl;

		system_info << "--- CURRENT MODULE ---" << std::endl;

		std::string module_name = utils::get_process_file_name();

#if LOG_USE_MODULEDEFINITION
		std::string detected_name = module_definition::module_name_by_addr((void*)&query_system_info);
		if (detected_name.length())
			module_name = detected_name;
#endif //LOG_USE_MODULEDEFINITION

		system_info << "Module Name: " << module_name << std::endl;

#if LOG_USE_MODULEDEFINITION && defined(LOG_PLATFORM_WINDOWS)
		HMODULE module = reinterpret_cast<HMODULE>(pe_image::api_find_module_entry_point(
			reinterpret_cast<intptr_t>(logging_get_caller_address())));
		
		std::string file_version, file_description;
        std::string product_version, company_name;
        unsigned long lang_id = 0;

        if (module_definition::module_query_version_info(module, file_version, file_description, product_version, company_name, lang_id))
		{
			if (file_version.size())
				system_info << "Module file version: " << file_version << std::endl;

			if (file_description.size())
				system_info << "Module file description: " << file_description << std::endl;

			if (product_version.size())
				system_info << "Product version: " << product_version << std::endl;

			if (company_name.size())
				system_info << "Module company name: " << company_name << std::endl;

			if (lang_id)
				system_info << "Module language ID: " << stringformat("%.X", lang_id) << std::endl;
		}
#endif //LOG_USE_MODULEDEFINITION && defined(LOG_PLATFORM_WINDOWS)

#ifdef LOG_PLATFORM_WINDOWS
		system_info << "--- SYSTEM PATHS ---" << std::endl;
		
		char buffer[MAX_PATH+1];
		GetWindowsDirectoryA(buffer, MAX_PATH);
		system_info << "Windows dir: " << buffer << std::endl;

		GetSystemDirectoryA(buffer, MAX_PATH);
		system_info << "System dir: " << buffer << std::endl;

		GetTempPathA(MAX_PATH, buffer);
		system_info << "Temp dir: " << buffer << std::endl;
#endif //LOG_PLATFORM_WINDOWS

		system_info << "********************************************" << std::endl;
		return system_info.str();
	}
#endif // LOG_USE_SYSTEMINFO
	
////////////////////////////////////////////


static const char* default_hdr_format = "[$(V)] $(dd).$(MM).$(yyyy) $(hh):$(mm):$(ss).$(ttt) [$(PID):$(TID)] [$(module)!$(function)]";

class log_configurator
{
public:
	log_configurator()
        :log_file_name_(utils::get_process_file_name() + ".log"),
		log_path_(utils::get_process_file_path()),
        hdr_format_(default_hdr_format),
        need_sys_info_(true),
		verb_level_(logger_verbose_optimal),
		scroll_file_size_(2097152),
		scroll_file_count_(15),
		scroll_file_every_run_(false)
#if LOG_CONFIGURE_FROM_REGISTRY
		,reg_config_path_("")
#endif //LOG_CONFIGURE_FROM_REGISTRY

#if LOG_INI_CONFIGURATION
		,ini_file_find_paths_(process_config_macro(LOG_DEFAULT_INI_PATHS))
#endif //LOG_INI_CONFIGURATION
	{
#if LOG_UNHANDLED_EXCEPTIONS
		init_unhandled_exceptions_handler();
#endif //LOG_UNHANDLED_EXCEPTIONS
	}

	const std::string& get_hdr_format() const { return hdr_format_; }
	void set_hdr_format(const std::string& headerFormat) { hdr_format_ = process_config_macro(headerFormat); };

#if LOG_USE_SYSTEMINFO
	void set_need_sys_info(bool needSystemInfo) { need_sys_info_ = needSystemInfo; }
	bool get_need_sys_info() const { return need_sys_info_; };
#endif //LOG_USE_SYSTEMINFO

	void set_log_file_name(std::string fileName) { log_file_name_ = process_config_macro(fileName); cached_log_file_path_.clear(); }
	std::string get_log_file_name() const { return log_file_name_; }

	void set_verbose_level(int verboseLevel) { verb_level_ = verboseLevel; }
	int get_verbose_level() const { return verb_level_; }

	void set_log_path(std::string logPath) { log_path_ = process_config_macro(logPath); cached_log_file_path_.clear(); }
	std::string get_log_path() const { return log_path_; }

	void set_log_scroll_file_size(size_t scrollFileSize) { scroll_file_size_ = scrollFileSize; }
	size_t get_log_scroll_file_size() const { return scroll_file_size_; }

	void set_log_scroll_file_count(size_t scrollFileCount) { scroll_file_count_ = scrollFileCount; }
	size_t get_log_scroll_file_count() const { return scroll_file_count_; }

	void set_log_scroll_file_every_run(bool force_scroll) { scroll_file_every_run_ = force_scroll; }
	bool get_log_scroll_file_every_run() const { return scroll_file_every_run_; }

	const std::string& get_full_log_file_path() 
	{
		if (!cached_log_file_path_.size())
			cached_log_file_path_ = get_log_path() + "/" + get_log_file_name();

		return cached_log_file_path_;
	}

#if LOG_CONFIGURE_FROM_REGISTRY
	void set_reg_config_path(std::string registryConfigurationPath) { reg_config_path_ = process_config_macro(registryConfigurationPath); }
	std::string get_reg_config_path() const { return reg_config_path_; }
#endif //LOG_CONFIGURE_FROM_REGISTRY

#if LOG_INI_CONFIGURATION
	void set_ini_file_find_paths(std::string ini_file_find_paths) { ini_file_find_paths_ = process_config_macro(ini_file_find_paths); }
	std::string get_ini_file_find_paths() const { return ini_file_find_paths_; }
#endif //LOG_INI_CONFIGURATION

private:
    static std::string process_config_macro(std::string str)
	{
        if (contains(str.c_str(),"$(CURRENTDIR)"))
        {
#ifdef LOG_PLATFORM_WINDOWS
            char current_dir[MAX_PATH+1];
            GetCurrentDirectoryA(MAX_PATH, current_dir);
#else //LOG_PLATFORM_WINDOWS
            char current_dir[512];

#	ifdef LOG_HAVE_UNISTD_H
            if (getcwd(current_dir,512))
                current_dir[0] = 0;
#	else //LOG_HAVE_UNISTD_H
	    strcpy(current_dir,"./");
#	endif //LOG_HAVE_UNISTD_H

#endif //LOG_PLATFORM_WINDOWS

            str = replace(str,"$(CURRENTDIR)", current_dir);
        }

        if (contains(str.c_str(),"$(TEMPPATH)"))
        {
#ifdef LOG_PLATFORM_WINDOWS
            char temp_path[MAX_PATH+1];
            GetTempPathA(MAX_PATH, temp_path);
#else //LOG_PLATFORM_WINDOWS
            const char* temp_path = P_tmpdir;
#endif //LOG_PLATFORM_WINDOWS

            str = replace(str,"$(TEMPPATH)", temp_path);
        }


#ifdef LOG_PLATFORM_WINDOWS
		if (contains(str.c_str(),"$(SYSTEMPATH)"))
		{
			char system_path[MAX_PATH+1];
			GetSystemDirectoryA(system_path, MAX_PATH);

			str = replace(str,"$(SYSTEMPATH)", system_path);
		}

		if (contains(str.c_str(),"$(WINDOWSPATH)"))
		{
			char windows_path[MAX_PATH+1];
			GetWindowsDirectoryA(windows_path, MAX_PATH);

			str = replace(str,"$(WINDOWSPATH)", windows_path);
		}

		if (contains(str.c_str(), "$(COMMONAPPDATA)"))
		{
			char appdata_path[MAX_PATH];
			appdata_path[0] = 0;

			SHGetFolderPathA(NULL, CSIDL_COMMON_APPDATA, NULL, 0, appdata_path);

			str = replace(str,"$(COMMONAPPDATA)", appdata_path);
		}

		if (contains(str.c_str(), "$(USERAPPDATA)"))
		{
			char appdata_path[MAX_PATH];
			appdata_path[0] = 0;

			SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appdata_path);

			str = replace(str,"$(USERAPPDATA)", appdata_path);
		}

		if (contains(str.c_str(), "$(DESKTOPDIR)"))
		{
			char desktop_path[MAX_PATH];
			desktop_path[0] = 0;

			SHGetFolderPathA(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, desktop_path);

			str = replace(str,"$(DESKTOPDIR)", desktop_path);
		}
#endif //LOG_PLATFORM_WINDOWS

		str = replace(str,"$(EXEDIR)", utils::get_process_file_path());
		str = replace(str,"$(EXEFILENAME)", utils::get_process_file_name());
		str = replace(str,"$(EXEFULLFILENAME)", utils::get_process_full_file_name());

#if LOG_USE_MODULEDEFINITION
        std::string module_path = module_definition::module_name_by_addr((void*)&process_config_macro);

        size_t last_delim = module_path.find_last_of("\\/");

		std::string module_dir = module_path.substr(0,last_delim);

		if (last_delim != std::string::npos)
			last_delim++;

		std::string module_full_file_name = module_path.substr(last_delim,std::string::npos);

		size_t count = module_path.find_last_of(".");
		if (count != std::string::npos)
			count -= last_delim;

		std::string module_file_name = module_path.substr(last_delim, count);

		str = replace(str,"$(MODULEDIR)", module_dir);
		str = replace(str,"$(MODULEFULLFILENAME)", module_full_file_name);
		str = replace(str,"$(MODULEFILENAME)", module_file_name);
#endif //LOG_USE_MODULEDEFINITION

		return str;
	}

	std::string log_file_name_;
    std::string log_path_;
    std::string hdr_format_;
	bool need_sys_info_;
	int verb_level_;
	size_t scroll_file_size_;
	size_t scroll_file_count_;
	bool scroll_file_every_run_;

	std::string cached_log_file_path_;

#if LOG_CONFIGURE_FROM_REGISTRY
	std::string reg_config_path_;
#endif //LOG_CONFIGURE_FROM_REGISTRY

#if LOG_INI_CONFIGURATION
	std::string ini_file_find_paths_;
#endif //LOG_INI_CONFIGURATION
};


#if LOG_TEST_DO_NOT_WRITE_FILE
#	include <sstream>
#	undef LOG_FLUSH_FILE_EVERY_WRITE
#	define LOG_FLUSH_FILE_EVERY_WRITE 1
#endif //LOG_TEST_DO_NOT_WRITE_FILE


extern log_configurator configurator;

//////////////////////////////////////////////////////////////
#if LOG_INI_CONFIGURATION
class log_ini_configurator
{
public:
	static bool configure(const char* ini_file_paths)
	{
		std::vector<std::string> ini_paths;
		split(ini_file_paths, ini_paths, ';');

		for (size_t i=0; i<ini_paths.size(); i++)
		{
		    if (logging_ini::ini_parse(ini_paths[i].c_str(), handler, NULL) >= 0) 
				return true;
		}

		return false;
	}

    static int LOG_CDECL handler(void* user, const char* section, const char* name,
					   const char* value)
	{
        (void)user;

		if (!strcmp(section,"logger") && !strcmp(name, "LogPath")) 
		{
			configurator.set_log_path(value);
		} 
		else if (!strcmp(section,"logger") && !strcmp(name, "Verbose")) 
		{
			configurator.set_verbose_level(atoi(value));
		} 
		else if (!strcmp(section,"logger") && !strcmp(name, "HeaderFormat")) 
		{
			configurator.set_hdr_format(value);
		} 
		else if (!strcmp(section,"logger") && !strcmp(name, "LogFileName")) 
		{
			configurator.set_log_file_name(value);
		} 
#if LOG_USE_SYSTEMINFO
        else if (!strcmp(section,"logger") && !strcmp(name, "LogSysInfo"))
		{
			configurator.set_need_sys_info(atoi(value) ? true : false);
		} 
#endif //LOG_USE_SYSTEMINFO
		else if (!strcmp(section,"logger") && !strcmp(name, "ScrollFileCount")) 
		{
			configurator.set_log_scroll_file_count(atoi(value));
		} 
		else if (!strcmp(section,"logger") && !strcmp(name, "ScrollFileSize")) 
		{
			configurator.set_log_scroll_file_size(atoi(value));
		} 
		else if (!strcmp(section,"logger") && !strcmp(name, "ScrollFileEveryRun")) 
		{
			configurator.set_log_scroll_file_every_run(atoi(value) ? true : false);
		} 
#if LOG_CONFIGURE_FROM_REGISTRY
        else if (!strcmp(section,"logger") && !strcmp(name, "RegistryConfigPath"))
		{
			configurator.set_reg_config_path(value);
		} 
#endif //LOG_CONFIGURE_FROM_REGISTRY
		else {
			return 0;  /* unknown section/name, error */
		}
		return 1;
	}

};

#endif //LOG_INI_CONFIGURATION

#if LOG_CONFIGURE_FROM_REGISTRY

class log_registry_configurator
{
public:
	static bool configure(std::string key_path)
	{
		if (key_path == "")
			key_path = LOG_REGISTRY_DEFAULT_KEY;

		size_t storage_delim = key_path.find_first_of("\\/");
		if (storage_delim == key_path.npos)
			return false;

		std::string storage = key_path.substr(0,storage_delim);
		HKEY base_key = log_registry_helper::reg_base_key_by_string(storage);
		
		std::string path = key_path.substr(storage_delim+1);

		std::string log_path;
		if (log_registry_helper::get_reg_sz_value(base_key,path,"LogPath",log_path))
			configurator.set_log_path(log_path);

		std::string hdr_format;
		if (log_registry_helper::get_reg_sz_value(base_key,path,"HeaderFormat",hdr_format))
			configurator.set_hdr_format(hdr_format);

		unsigned long verbose_level;
		if (log_registry_helper::get_reg_dword_value(base_key,path,"Verbose",verbose_level))
			configurator.set_verbose_level(verbose_level);

#if LOG_USE_SYSTEMINFO
		unsigned long log_sys_info;
		if (log_registry_helper::get_reg_dword_value(base_key,path,"LogSystemInfo",log_sys_info))
			configurator.set_need_sys_info(log_sys_info ? true : false);
#endif //LOG_USE_SYSTEMINFO

		unsigned long log_scroll_file_size;
		if (log_registry_helper::get_reg_dword_value(base_key,path,"ScrollFileSize",log_scroll_file_size))
			configurator.set_log_scroll_file_size(log_scroll_file_size);

		unsigned long log_scroll_file_count;
		if (log_registry_helper::get_reg_dword_value(base_key,path,"ScrollFileCount",log_scroll_file_count))
			configurator.set_log_scroll_file_count(log_scroll_file_count);

		unsigned long log_scroll_file_every_run;
		if (log_registry_helper::get_reg_dword_value(base_key,path,"ScrollFileEveryRun",log_scroll_file_every_run))
			configurator.set_log_scroll_file_every_run(log_scroll_file_every_run ? true : false);

		unsigned long log_enabled;
		if (log_registry_helper::get_reg_dword_value(base_key,path,"LogEnabled",log_enabled))
		{
			if (!log_enabled)
				configurator.set_verbose_level(logger_verbose_mute);
		}

		return true;
	}
	
};

#endif //LOG_CONFIGURE_FROM_REGISTRY

//////////////////////////////////////////////////////////////

struct logger_interface
{
	virtual ~logger_interface() {};

	virtual void log_binary(int verbLevel, void* addr, const char* functionName, 
		const char* sourceFile, int lineNumber, const char* data, int len) = 0;

    virtual void /*__cdecl*/ log(int verbLevel, void* addr, const char* functionName,
		const char* sourceFile, int lineNumber, const char* format, ...) = 0;

	virtual void log_args(int verb_level, void* addr, const char* functionName, 
		const char* sourceFile, int lineNumber, const char* format, va_list arguments) = 0;

#if LOG_USE_MODULEDEFINITION
	virtual void log_modules(int verbLevel, void* addr, const char* functionName, 
		const char* sourceFile, int lineNumber) = 0;
#endif //LOG_USE_MODULEDEFINITION

#if LOG_AUTO_DEBUGGING
	virtual void log_stack_trace(int verbLevel, void* addr, const char* functionName, 
		const char* sourceFile, int lineNumber) = 0;
#endif //LOG_AUTO_DEBUGGING

	virtual void log_exception(int verbLevel, void* addr, const char* functionName, 
		const char* sourceFile, int lineNumber, const char* userMessage) = 0;

	virtual void log_exception(int verbLevel, void* addr, const char* functionName, 
		const char* sourceFile, int lineNumber, std::exception* e) = 0;

	virtual void ref() = 0;
	virtual void deref() = 0;
	virtual int ref_counter() = 0;
};

//////////////////////////////////////////////////////////////

#if LOG_AUTO_DEBUGGING

class runtime_debugging
{


#if defined(LOG_PLATFORM_WINDOWS)

private:
	static std::string get_pdb_search_path()
	{
		char pathBuffer[MAX_PATH];
		std::stringstream symSearchPath;

		const char* PdbCurrDir = "\\pdb";
		const int PdbCurrDirLength = 5;

		if (GetCurrentDirectoryA(sizeof(pathBuffer), pathBuffer))
		  symSearchPath << pathBuffer << ";";

		if (strlen(pathBuffer) < sizeof(pathBuffer) - PdbCurrDirLength)
		{
			strcat(pathBuffer, PdbCurrDir);
			symSearchPath << pathBuffer << ";";
		}

		symSearchPath << utils::get_process_file_path() << ";";
		symSearchPath << utils::get_process_file_path() << PdbCurrDir << ";";

		symSearchPath << configurator.get_log_path() << ";";
		symSearchPath << configurator.get_log_path() << PdbCurrDir << ";";

		if (strlen(pathBuffer) && strlen(pathBuffer) < sizeof(pathBuffer) - PdbCurrDirLength)
		{
			strcat(pathBuffer, PdbCurrDir);
		}

		if (GetEnvironmentVariableA("_NT_SYMBOL_PATH", pathBuffer, sizeof(pathBuffer)))
		  symSearchPath << pathBuffer << ";";

		if (GetEnvironmentVariableA("_NT_ALTERNATE_SYMBOL_PATH", pathBuffer, sizeof(pathBuffer)))
		  symSearchPath << pathBuffer << ";";

		if (GetEnvironmentVariableA("SYSTEMROOT", pathBuffer, sizeof(pathBuffer)))
		  symSearchPath << pathBuffer << ";";

		std::string result = symSearchPath.str();
		if (result.size() > 0)
			result = result.substr(0,result.size()-1);

		return result;
	}

	static const char* get_symbol_type_string(int sym_type)
	{
		switch (sym_type)
		{
			case SymNone: return "-nosymbols-"; 
			case SymCoff: return "COFF";
			case SymCv: return "CV";
			case SymPdb: return "PDB";
			case SymExport: return "-exported-";
			case SymDeferred: return "-deferred-";
			case SymSym: return "SYM";
			default: return stringformat("symtype=%ld",sym_type).c_str();
		}
	}

	static void init_current_process_symbols()
	{
		static bool isFirstTime = TRUE;
		if (isFirstTime) 
		{
			if (!::SymInitialize(GetCurrentProcess(), get_pdb_search_path().c_str(), false))
				return;

			unsigned long symOptions = SymGetOptions();
			symOptions |= SYMOPT_LOAD_LINES;
			symOptions &= ~SYMOPT_UNDNAME;
			symOptions &= ~SYMOPT_DEFERRED_LOADS;
			SymSetOptions(symOptions);

			std::list<module_entry_t> modules;
			module_definition::query_module_list(modules);
			load_modules_pdbs(modules);

			isFirstTime = FALSE;
		} 
	}

	static void load_modules_pdbs(const std::list<module_entry_t>& modules)
	{
		for (std::list<module_entry_t>::const_iterator it = modules.begin(); it != modules.end(); ++it)
		{
			SymLoadModule(GetCurrentProcess(), 0, it->imageName.c_str(), it->moduleName.c_str(), 
				it->baseAddress, it->size );
		}
	}

public:
	static const char* get_system_expection_code_text(unsigned long exceptionCode) 
	{
		switch(exceptionCode) 
		{
			case EXCEPTION_ACCESS_VIOLATION: return "ACCESS VIOLATION";
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "ARRAY BOUNDS EXCEEDED";
			case EXCEPTION_BREAKPOINT: return "BREAKPOINT";
			case EXCEPTION_DATATYPE_MISALIGNMENT: return "DATATYPE MISALIGNMENT";
			case EXCEPTION_FLT_DENORMAL_OPERAND: return "FLT DENORMAL OPERAND";
			case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "FLT DIVIDE BY ZERO";
			case EXCEPTION_FLT_INEXACT_RESULT: return "FLT INEXACT RESULT";
			case EXCEPTION_FLT_INVALID_OPERATION: return "FLT INVALID OPERATION";
			case EXCEPTION_FLT_OVERFLOW: return "FLT OVERFLOW";
			case EXCEPTION_FLT_STACK_CHECK: return "FLT STACK CHECK";
			case EXCEPTION_FLT_UNDERFLOW: return "FLT UNDERFLOW";
			case EXCEPTION_ILLEGAL_INSTRUCTION: return "ILLEGAL INSTRUCTION";
			case EXCEPTION_IN_PAGE_ERROR: return "IN PAGE ERROR";
			case EXCEPTION_INT_DIVIDE_BY_ZERO: return "INT DIVIDE BY ZERO";
			case EXCEPTION_INT_OVERFLOW: return "INT OVERFLOW";
			case EXCEPTION_INVALID_DISPOSITION: return "INVALID DISPOSITION";
			case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "NONCONTINUABLE EXCEPTION";
			case EXCEPTION_PRIV_INSTRUCTION: return "PRIV INSTRUCTION";
			case EXCEPTION_SINGLE_STEP: return "SINGLE STEP";
			case EXCEPTION_STACK_OVERFLOW: return "STACK OVERFLOW";
			case DBG_CONTROL_C : return "DBG CONTROL C ";
			default:
			return "<unkown exception>";
		}
	}
	
	struct stack_frame_t
	{
		std::string undecorated_sym_name;
		std::string undecorated_full_sym_name;

		DWORD offset_from_line_bytes;
		
		platform_dword_t offset_from_symbol_bytes;
		platform_dword_t program_counter;
		platform_dword_t module_base;

		const char* sym_type;

		std::string module_file_name;
		std::string module_name;

		std::string src_file_name;
		int src_file_line;

		stack_frame_t() : offset_from_symbol_bytes(0), sym_type(NULL), src_file_line(0), 
			offset_from_line_bytes(0), program_counter(0), module_base(0)
		{}
	};

	static std::list<stack_frame_t> get_stack_frames(HANDLE threadHandle, CONTEXT& c) 
	{
		static CRITICAL_SECTION cs;
		static bool cs_initialized = false;

		if (!cs_initialized)
		{
			InitializeCriticalSection(&cs);
			cs_initialized = true;
		}

		EnterCriticalSection(&cs);

#ifdef _AMD64_
		const intptr_t debug_failed_rip = 0xCCCCCCCCCCCCCCCC;
		DWORD image_type = IMAGE_FILE_MACHINE_AMD64; 
#else //_AMD64_
		const intptr_t debug_failed_rip = 0xCCCCCCCC;
		DWORD image_type = IMAGE_FILE_MACHINE_I386;
#endif //_AMD64_
		HANDLE process_handle = GetCurrentProcess();

		std::list<stack_frame_t> frames;

		static IMAGEHLP_SYMBOL *sym_ptr = NULL;
		if (sym_ptr == NULL) 
		{
			sym_ptr = (IMAGEHLP_SYMBOL*) malloc(MAX_PATH + sizeof(IMAGEHLP_SYMBOL));
			if (!sym_ptr) 
			{
				LeaveCriticalSection(&cs);
				return frames;
			}
		}

		init_current_process_symbols();

#ifndef _AMD64_
		STACKFRAME stack_frame;
#else //_AMD64_
		STACKFRAME64 stack_frame;
#endif //_AMD64_

		memset(&stack_frame, 0, sizeof(stack_frame));

#ifndef _AMD64_
		stack_frame.AddrPC.Offset = c.Eip;
		stack_frame.AddrFrame.Offset = c.Ebp;
		stack_frame.AddrStack.Offset = c.Esp;
#else //_AMD64_
		stack_frame.AddrPC.Offset = c.Rip;
		stack_frame.AddrFrame.Offset = c.Rbp;
		stack_frame.AddrStack.Offset = c.Rsp;
#endif //_AMD64_
		stack_frame.AddrPC.Mode = AddrModeFlat;
		stack_frame.AddrFrame.Mode = AddrModeFlat;
		stack_frame.AddrStack.Mode = AddrModeFlat;

		memset(sym_ptr, 0, MAX_PATH + sizeof(IMAGEHLP_SYMBOL));
		sym_ptr->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
		sym_ptr->MaxNameLength = MAX_PATH;

		IMAGEHLP_LINE line_info;
		memset(&line_info, 0, sizeof line_info);
		line_info.SizeOfStruct = sizeof(line_info);

		IMAGEHLP_MODULE module_info;
		memset(&module_info, 0, sizeof(module_info));
		module_info.SizeOfStruct = sizeof(module_info);

		for (int walkedStackFrames = 0; ; ++walkedStackFrames)
		{
//WARNING: in some cases we need to provide NULL as context when running in x86 context. But in most cases it does not working, try to provide context
//			in all cases

			if (!::StackWalk(image_type, process_handle, threadHandle, &stack_frame,  /*image_type == IMAGE_FILE_MACHINE_I386 ? NULL :*/ &c, NULL, 
				&::SymFunctionTableAccess, &::SymGetModuleBase, NULL ))
			{
				break;
			}

			stack_frame_t frame;
			frame.sym_type = get_symbol_type_string(SymNone);

			if ( stack_frame.AddrPC.Offset != 0 
				&& stack_frame.AddrPC.Offset != debug_failed_rip)
			{
				frame.program_counter = stack_frame.AddrPC.Offset;

				if (::SymGetSymFromAddr(process_handle, stack_frame.AddrPC.Offset, &frame.offset_from_symbol_bytes, sym_ptr))
				{
					char undName[MAX_PATH] = {0};
					char undFullName[MAX_PATH] = {0};

					UnDecorateSymbolName(sym_ptr->Name, undName, MAX_PATH, UNDNAME_NAME_ONLY);
					UnDecorateSymbolName(sym_ptr->Name, undFullName, MAX_PATH, UNDNAME_COMPLETE);

					frame.undecorated_sym_name = strlen(undName) > 0 ? undName : sym_ptr->Name;
					frame.undecorated_full_sym_name = undFullName;
				}

				if (::SymGetLineFromAddr(process_handle, stack_frame.AddrPC.Offset, &frame.offset_from_line_bytes, &line_info))
				{
					frame.src_file_name = line_info.FileName;
					frame.src_file_line = line_info.LineNumber;
				}

				if (::SymGetModuleInfo(process_handle, stack_frame.AddrPC.Offset, &module_info))
				{
					frame.module_name = module_info.ModuleName;
					frame.module_base = module_info.BaseOfImage;
					frame.sym_type = get_symbol_type_string(module_info.SymType);
					frame.module_file_name = module_info.LoadedImageName;
				}
			} 
			frames.push_back(frame);
		} 

		LeaveCriticalSection(&cs);
		return frames;
	}

	static std::string get_stack_trace_string(HANDLE threadHandle, CONTEXT& c, int ignoreFunctions = 0) 
	{
		std::stringstream stream;
		std::list<stack_frame_t> stackFrames = get_stack_frames(threadHandle,c);

		int n = 0;

		for(std::list<stack_frame_t>::iterator i=stackFrames.begin(); i!=stackFrames.end(); i++, n++)
		{
			if (ignoreFunctions-- > 0)
				continue;
			
			stack_frame_t& frame = *i;

			stream << "[" << n << "] " << "<--";
			if (frame.undecorated_sym_name.size())
				stream << stringformat(" %s %+ld bytes ", frame.undecorated_sym_name.c_str(), frame.offset_from_symbol_bytes);

			if (frame.undecorated_full_sym_name.size())
				stream << stringformat("[FUNCTION: %s] ", frame.undecorated_full_sym_name.c_str()); 

			if (frame.src_file_name.size() || frame.src_file_line)
				stream << stringformat("[Line: %s(%lu) %+ld bytes] ", frame.src_file_name.c_str(), frame.src_file_line, frame.offset_from_line_bytes);

			if (frame.module_name.size())
			{
#ifdef _AMD64_
                stream << stringformat("[Mod:  %s, base: %.8X%.8X] ",frame.module_name.c_str(), (uint32_t)(frame.module_base >> 32), (uint32_t)frame.module_base);
#else //_AMD64_
				stream << stringformat("[Mod:  %s, base: %08lxh] ",frame.module_name.c_str(), frame.module_base);
#endif //_AMD64_
			}

#if LOG_STACKTRACE_DETECT_MODNAME_IF_NOT_FOUND
			if (!frame.module_name.size() && frame.program_counter)
			{
				std::string mod_detected_name = module_definition::module_name_by_addr(reinterpret_cast<void*>(frame.program_counter));
				if (mod_detected_name.size())
					stream << stringformat("[DETECTED Mod:  %s] ", mod_detected_name.c_str());
			}
#endif //LOG_STACKTRACE_DETECT_MODNAME_IF_NOT_FOUND

#ifdef _AMD64_
            stream << stringformat("\n Offset: 0x%.8X%.8X", (uint32_t)(frame.program_counter >> 32), (uint32_t)frame.program_counter);
#else //_AMD64_
            stream << stringformat("\n Offset: 0x%8.8X", frame.program_counter);
#endif //_AMD64_

			stream << stringformat(" Sym:  type: %s, file: %s\n", frame.sym_type, frame.module_file_name.c_str());
		}
		return stream.str();
	}

	static std::string get_stack_trace_string(CONTEXT* c, int ignoreFunctions = 0)
	{
		return get_stack_trace_string(NULL,*c,ignoreFunctions);
	}


#else //defined(LOG_PLATFORM_WINDOWS)

public:
    static std::string get_stack_trace_string(void* trace[], int trace_len, int ignore_functions = 0)
    {
        std::string result;
        char **bt_syms;
        int i;

        size_t funcnamesize = 256;
        char* funcname = (char*)malloc(funcnamesize);

        bt_syms = backtrace_symbols(trace, trace_len);

        int index = 1;

        for (i = 1 + ignore_functions; i < trace_len; i++)
        {
            std::string cur_full_sym = bt_syms[i];
            char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

            result += stringformat(" [%d] <-- (%p) ", index++, trace[i]);

            // find parentheses and +address offset surrounding the mangled name:
            // ./module(function+0x15c) [0x8048a6d]
            for (char *p = bt_syms[i]; *p; ++p)
            {
                if (*p == '(')
                    begin_name = p;
                else if (*p == '+')
                    begin_offset = p;
                else if (*p == ')' && begin_offset)
                {
                    end_offset = p;
                    break;
                }
            }

            if (begin_name && begin_offset && end_offset
                && begin_name < begin_offset)
            {
                *begin_name++ = '\0';
                *begin_offset++ = '\0';
                *end_offset = '\0';

                int status;
                char* ret = abi::__cxa_demangle(begin_name,
                                funcname, &funcnamesize, &status);
                if (status == 0)
                {
                    funcname = ret; // use possibly realloc()-ed string
                    result += stringformat(" %s: %s+%s\n", bt_syms[i], funcname, begin_offset);
                }
                else
                {
                    // demangling failed. Output function name as a C function with
                    // no arguments.
                    result += stringformat(" %s: %s()+%s\n",bt_syms[i], begin_name, begin_offset);
                }
            }
            else
            {
                // couldn't parse the line? print the whole line.
                result += stringformat(" %s\n", bt_syms[i]);
            }

            result += "    ";
            result += cur_full_sym;
            result += "\n";
        }

        free(bt_syms);
        free(funcname);


        return result;
    }

#endif //defined(LOG_PLATFORM_WINDOWS)

};
#endif //LOG_AUTO_DEBUGGING

//////////////////////////////////////////////////////////////

#if LOG_SHARED

struct shared_obj
{
	static const intptr_t start_shared_page = 0x04320000;
	static const int shared_page_size = 0x100000;
	static const int shared_pages_find = 0x30;
	static const unsigned long shared_page_mem_size = 0x100;
	static const unsigned long shared_mem_signature_1 = 0x12345678;
	static const unsigned long shared_mem_signature_2 = 0xA0B0C0D0;


#ifndef LOG_PLATFORM_WINDOWS
    static inline bool get_page_bits(void *addr, int *bits)
    {
	// On linux, first check /proc/self/maps
	unsigned long laddr = reinterpret_cast<unsigned long>(addr);

	FILE *pF = fopen("/proc/self/maps", "r");
	if (pF) {
	    // Linux /proc/self/maps -> parse
	    // Format:
	    // lower    upper    prot     stuff                 path
	    // 08048000-0804c000 r-xp 00000000 03:03 1010107    /bin/cat
	    unsigned long rlower, rupper;
	    char r, w, x;
	    while (fscanf(pF, "%lx-%lx %c%c%c", &rlower, &rupper, &r, &w, &x) != EOF) {
		// Check whether we're IN THERE!
		if (laddr >= rlower && laddr < rupper) {
		    fclose(pF);
		    *bits = 0;
		    if (r == 'r')
			*bits |= SH_MEM_READ;
		    if (w == 'w')
			*bits |= SH_MEM_WRITE;
		    if (x == 'x')
			*bits |= SH_MEM_EXEC;

		    return true;
		}
		// Read to end of line
		int c;
		while ((c = fgetc(pF)) != '\n') {
		    if (c == EOF)
			break;
		}
		if (c == EOF)
		    break;
	    }
	    fclose(pF);
	    return false;
	}
	
	pF = fopen("/proc/curproc/map","r");
	if (pF) {
	    // FreeBSD /proc/curproc/map
	    // 0x804800 0x805500 13 15 0xc6e18960 r-x 21 0x0 COW NC vnode
	    unsigned long rlower, rupper, ignoreLong;
	    int ignoreInt;
	    char r,w,x;
	    while(fscanf(pF, "0x%lx 0x%lx %d %d 0x%lx %c%c%c", &rlower, &rupper, &ignoreInt, 
		&ignoreInt, &ignoreLong, &r, &w, &x) != EOF)
	    {
		if(laddr >= rlower && laddr < rupper)
		{
		    fclose(pF);
		    *bits = 0;
		    if (r == 'r')
			*bits |= SH_MEM_READ;
		    if (w == 'w')
			*bits |= SH_MEM_WRITE;
		    if (x == 'x')
			*bits |= SH_MEM_EXEC;
		    return true;
		}

		// Read to end of line
		int c;
		while ((c = fgetc(pF)) != '\n')
		{
		    if (c == EOF)
			break;
		}
		if (c == EOF)
		    break;
	    }
	    fclose(pF);
	    return false;
	}
	return false;
    }
#endif //LOG_PLATFORM_WINDOWS

	static void* create_shared_object(unsigned char object_id, void* object)
	{
		intptr_t shared_page = get_start_address(object_id);
		void* result_ptr = NULL;
		
		for (intptr_t page = shared_page; 
			page < shared_page + (shared_pages_find*shared_page_size); 
			page += shared_page_size)
		{
#ifdef LOG_PLATFORM_WINDOWS
			MEMORY_BASIC_INFORMATION mem_info;
			if (!VirtualQuery((LPCVOID)page, &mem_info, sizeof(MEMORY_BASIC_INFORMATION)))
				continue;

			if (mem_info.State != MEM_FREE)
				continue;

			result_ptr = VirtualAlloc((LPVOID)page, shared_page_mem_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else //LOG_PLATFORM_WINDOWS
			int page_bits;

			if (get_page_bits((void*)page, &page_bits))
			    continue;

			result_ptr = mmap((void*)page, shared_page_mem_size, PROT_READ|PROT_WRITE|PROT_EXEC,MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS,-1,0);
#endif //LOG_PLATFORM_WINDOWS

			if (result_ptr)
			{
				memset(result_ptr, 0, shared_page_mem_size);

				unsigned long* ptr = (unsigned long*)page;
				*ptr = shared_mem_signature_1;

				ptr++;

				*ptr = shared_mem_signature_2;

				*(intptr_t*)(page + 8) = (intptr_t)object;

				break;
			}
		}
		
		return result_ptr;
	}

	static intptr_t try_found_shared_object_page(unsigned short object_id)
	{
		for (intptr_t page = start_shared_page; 
			page < start_shared_page + (shared_pages_find*shared_page_size); 
			page += shared_page_size)
		{
#ifdef LOG_PLATFORM_WINDOWS
			MEMORY_BASIC_INFORMATION mem_info;
			if (!VirtualQuery((LPCVOID)page, &mem_info, sizeof(MEMORY_BASIC_INFORMATION)))
				continue;

			if (mem_info.State == MEM_COMMIT 
				&& mem_info.Protect & PAGE_READWRITE)
#else //LOG_PLATFORM_WINDOWS
			int page_bits;
			if (!get_page_bits((void*)page, &page_bits))
			    continue;

			if ((page_bits & SH_MEM_READ) && (page_bits & SH_MEM_WRITE))
#endif //LOG_PLATFORM_WINDOWS
			{
				if (*((unsigned long*)page) != shared_mem_signature_1)
					continue;

				if (*((unsigned long*)page + 1) != shared_mem_signature_2)
					continue;

				return page;
			}
		}
		return (intptr_t)NULL;
	}
	
	static void* try_found_shared_object(unsigned short object_id)
	{
		intptr_t page = try_found_shared_object_page(object_id);
		if (!page)
			return NULL;

		intptr_t* ptr = (intptr_t*)(page + 8);
		return (void*)*ptr;
	}

	static bool free_shared_object(unsigned char object_id)
	{
		void* obj = (void*)try_found_shared_object_page(object_id);
		if (!obj)
			return false;

#ifdef LOG_PLATFORM_WINDOWS
		VirtualFree(obj, 0, MEM_DECOMMIT);
		return VirtualFree(obj, 0, MEM_RELEASE) ? true : false;
#else //LOG_PLATFORM_WINDOWS
		return munmap(obj, shared_page_mem_size) == 0;
#endif //LOG_PLATFORM_WINDOWS
	}

private:
	static intptr_t get_start_address(unsigned char object_id)
	{
		return start_shared_page + ((intptr_t)object_id << 22);
	}	
};

#endif //LOG_SHARED


////////////////////  Logger implementation  //////////////////////////

class logger
	: public logger_interface
{
private:
	static int get_log_file_index(const std::string& name)
	{
		std::string idx = name.substr(name.find_last_of('.'));

		if (idx.size())
			idx = idx.substr(1);

		return atoi(idx.c_str());
	}

	int cur_file_size_;

	void scroll_files(bool force = false)
	{
		if (!force && !configurator.get_log_scroll_file_size())
			return;

        bool need_scroll = force;

		if (!need_scroll)
			need_scroll = cur_file_size_ > configurator.get_log_scroll_file_size();

		if (need_scroll)
		{
			std::map<int,std::string> log_files;
			std::vector<int> log_indexes;
            int max_index = 0;

			cur_file_size_ = 0;

#ifdef LOG_PLATFORM_WINDOWS
            WIN32_FIND_DATAA find_data;

            std::string mask = configurator.get_full_log_file_path() + ".*";
			HANDLE find_handle = FindFirstFileA(mask.c_str(), &find_data);


			if (find_handle != INVALID_HANDLE_VALUE)
			{
				do 
				{
					std::string name = find_data.cFileName;
					int index = get_log_file_index(name);

					if (index > max_index)
						max_index = index;

					if (index)
					{
						log_indexes.push_back(index);
						log_files.insert(std::pair<int,std::string>(index, name));
					}

				} while(FindNextFileA(find_handle, &find_data));

				FindClose(find_handle);
			}

#else //LOG_PLATFORM_WINDOWS

            std::string find_pattern = configurator.get_log_file_name() + ".*";

            DIR* pDirectory = opendir(configurator.get_log_path().c_str());
            if (pDirectory)
            {
                struct dirent* pFile = NULL;

              while (NULL != (pFile = readdir(pDirectory)))
              {
                  int ret = fnmatch(find_pattern.c_str(), pFile->d_name, FNM_PATHNAME | FNM_PERIOD);
                  if (ret == 0)
                  {
                      int index = get_log_file_index(pFile->d_name);

                      if (index > max_index)
                          max_index = index;

                      if (index)
                      {
                          log_indexes.push_back(index);
                          log_files.insert(std::pair<int,std::string>(index, pFile->d_name));
                      }

                  }
              }
              closedir(pDirectory);
            }
#endif //LOG_PLATFORM_WINDOWS

#if !LOG_FLUSH_FILE_EVERY_WRITE
			stream.flush();
			stream.close();
#endif //LOG_FLUSH_FILE_EVERY_WRITE

			for(unsigned int i=max_index; i>0; i--)
			{
				if (std::find(log_indexes.begin(),log_indexes.end(), i) == log_indexes.end())
					continue;

				unsigned int new_index = i + 1;
				std::string name = log_files[i];

				if (configurator.get_log_scroll_file_count() 
					&& configurator.get_log_scroll_file_count() < new_index)
				{
#ifdef LOG_PLATFORM_WINDOWS
                    DeleteFileA((configurator.get_log_path() + "/" + name).c_str());
#else //LOG_PLATFORM_WINDOWS

#	ifdef LOG_HAVE_UNISTD_H
                    unlink((configurator.get_log_path() + "/" + name).c_str());
#	else //LOG_HAVE_UNISTD_H
		    std::remove((configurator.get_log_path() + "/" + name).c_str());
#	endif //LOG_HAVE_UNISTD_H

#endif //LOG_PLATFORM_WINDOWS

					continue;
				}

				std::string new_name = name.substr(0, name.find_last_of('.')) + stringformat(".%d", new_index);

#ifdef LOG_PLATFORM_WINDOWS
                MoveFileA((configurator.get_log_path() + "/" + name).c_str(),
					(configurator.get_log_path() + "/" + new_name).c_str());
#else //LOG_PLATFORM_WINDOWS
                rename((configurator.get_log_path() + "/" + name).c_str(),
                       (configurator.get_log_path() + "/" + new_name).c_str());
#endif //LOG_PLATFORM_WINDOWS
			}

#ifdef LOG_PLATFORM_WINDOWS
			MoveFileExA(configurator.get_full_log_file_path().c_str(), (configurator.get_full_log_file_path() + ".1").c_str(), 
						MOVEFILE_WRITE_THROUGH | MOVEFILE_REPLACE_EXISTING);
#else //LOG_PLATFORM_WINDOWS
            rename(configurator.get_full_log_file_path().c_str(), (configurator.get_full_log_file_path() + ".1").c_str());
#endif //LOG_PLATFORM_WINDOWS

#if !LOG_FLUSH_FILE_EVERY_WRITE
			if (!stream.is_open())   
				stream.open(configurator.get_full_log_file_path().c_str(),std::ios::app);
#endif //LOG_FLUSH_FILE_EVERY_WRITE
		}
	}

	int ref_counter_;

#if LOG_SHARED
	bool shared_master_;
	void* shared_obj_ptr_;
#endif //LOG_SHARED



#if LOG_MULTITHREADED

#	ifdef LOG_PLATFORM_WINDOWS
#		define LOG_MT_MUTEX CRITICAL_SECTION
#		define LOG_MT_MUTEX_INIT(x, y) InitializeCriticalSection(x)
#		define LOG_MT_MUTEX_LOCK(x) EnterCriticalSection(x)
#		define LOG_MT_MUTEX_UNLOCK(x) LeaveCriticalSection(x)
#		define LOG_MT_MUTEX_DESTROY(x) DeleteCriticalSection(x)
#		define LOG_MT_THREAD_EXIT(x)  ExitThread(x)
#	else //LOG_PLATFORM_WINDOWS

#		define LOG_MT_MUTEX pthread_mutex_t
#		define LOG_MT_MUTEX_INIT pthread_mutex_init
#		define LOG_MT_MUTEX_LOCK pthread_mutex_lock
#		define LOG_MT_MUTEX_UNLOCK pthread_mutex_unlock
#		define LOG_MT_MUTEX_DESTROY pthread_mutex_destroy
#		define LOG_MT_THREAD_EXIT pthread_exit


#	endif //LOG_PLATFORM_WINDOWS

#endif //LOG_MULTITHREADED



#if LOG_MULTITHREADED
	LOG_MT_MUTEX mt_buffer_lock;
	std::list<std::string> mt_buffer;

#	ifdef LOG_PLATFORM_WINDOWS
	HANDLE log_thread_handle;
	HANDLE write_event;
#	else //LOG_PLATFORM_WINDOWS
	pthread_t log_thread_handle;
	pthread_cond_t write_event;
#	endif //LOG_PLATFORM_WINDOWS

	int mt_terminating;

	static unsigned long 
#	ifdef LOG_PLATFORM_WINDOWS
		__stdcall 
#	endif //LOG_PLATFORM_WINDOWS
		log_thread_fn(void* data)
	{
		logger* log = reinterpret_cast<logger*>(data);
		
#if !LOG_FLUSH_FILE_EVERY_WRITE
		if (!log->stream.is_open())   
			log->stream.open(configurator.get_full_log_file_path().c_str(),std::ios::app);
#endif //LOG_FLUSH_FILE_EVERY_WRITE

		while(true)
		{
#ifdef LOG_PLATFORM_WINDOWS
			WaitForSingleObject(log->write_event, 100);
#endif //LOG_PLATFORM_WINDOWS

			LOG_MT_MUTEX_LOCK(&log->mt_buffer_lock);

#ifndef LOG_PLATFORM_WINDOWS
			pthread_cond_wait(&log->write_event, &log->mt_buffer_lock);
#endif //LOG_PLATFORM_WINDOWS

			while (log->mt_buffer.size())
			{
				log->scroll_files();
				std::string str = *log->mt_buffer.begin();
				log->cur_file_size_ += static_cast<int>(str.size());

#if LOG_FLUSH_FILE_EVERY_WRITE
#	if !LOG_TEST_DO_NOT_WRITE_FILE
				{
					std::ofstream stream(configurator.get_full_log_file_path().c_str(),std::ios::app);
					stream << str;
				}
#	endif //LOG_TEST_DO_NOT_WRITE_FILE
#else //LOG_FLUSH_FILE_EVERY_WRITE
				log->stream << str;
#endif //LOG_FLUSH_FILE_EVERY_WRITE

				log->mt_buffer.pop_front();
			}

			LOG_MT_MUTEX_UNLOCK(&log->mt_buffer_lock);

			if (log->mt_terminating)
			{
#if !LOG_FLUSH_FILE_EVERY_WRITE 
				log->stream.flush();
				log->stream.close();
#endif //LOG_FLUSH_FILE_EVERY_WRITE

				break;
			}
		}

		LOG_MT_THREAD_EXIT(0);
		return 0;
	}

	void put_to_stream(const std::string& what)
	{
		LOG_MT_MUTEX_LOCK(&mt_buffer_lock);
		mt_buffer.push_back(what);

#ifdef LOG_PLATFORM_WINDOWS
		SetEvent(write_event);
#else //LOG_PLATFORM_WINDOWS
		pthread_cond_signal(&write_event);
#endif //LOG_PLATFORM_WINDOWS

		LOG_MT_MUTEX_UNLOCK(&mt_buffer_lock);
	}

#else  //LOG_MULTITHREADED
	__inline void put_to_stream(const std::string& what)
	{
		scroll_files();

#if LOG_FLUSH_FILE_EVERY_WRITE
#	if !LOG_TEST_DO_NOT_WRITE_FILE
		std::ofstream stream(configurator.get_full_log_file_path().c_str(),std::ios::app);
		stream << what;
#	endif //LOG_TEST_DO_NOT_WRITE_FILE
#else  //LOG_FLUSH_FILE_EVERY_WRITE
		if (!stream.is_open())   
			stream.open(configurator.get_full_log_file_path().c_str(),std::ios::app);

		stream << what;
#endif //LOG_FLUSH_FILE_EVERY_WRITE
		cur_file_size_ += static_cast<int>(what.size());
	}
#endif //LOG_MULTITHREADED

#if !LOG_FLUSH_FILE_EVERY_WRITE
	std::ofstream stream;
#endif //LOG_FLUSH_FILE_EVERY_WRITE

public:
	void ref() { ref_counter_++; }
	void deref() { ref_counter_--; }
	int ref_counter() {	return ref_counter_; }

#if LOG_CREATE_DIRECTORY
#   ifndef LOG_PLATFORM_WINDOWS

    static int do_mkdir(const char *path, mode_t mode)
    {
        struct stat st;
        int             status = 0;

        if (stat(path, &st) != 0)
        {
            /* Directory does not exist. EEXIST for race condition */
            if (mkdir(path, mode) != 0 && errno != EEXIST)
                status = -1;
        }
        else if (!S_ISDIR(st.st_mode))
        {
            errno = ENOTDIR;
            status = -1;
        }

        return(status);
    }

    static int mkpath(const char* path, mode_t mode)
    {
        char           *pp;
        char           *sp;
        int             status;
        char           *copypath = strdup(path);

        status = 0;
        pp = copypath;
        while (status == 0 && (sp = strchr(pp, '/')) != 0)
        {
            if (sp != pp)
            {
                /* Neither root nor double slash in path */
                *sp = '\0';
                status = do_mkdir(copypath, mode);
                *sp = '/';
            }
            pp = sp + 1;
        }
        if (status == 0)
            status = do_mkdir(path, mode);
        free(copypath);
        return (status);
    }

#   endif //LOG_PLATFORM_WINDOWS
#endif //LOG_CREATE_DIRECTORY


	logger()
		:ref_counter_(0)
		,cur_file_size_(0)
#if LOG_SHARED
		, shared_master_(false)
		, shared_obj_ptr_(NULL)
#endif //LOG_SHARED

#if LOG_MULTITHREADED
		, mt_terminating(0)
#endif //LOG_MULTITHREADED
	{
#if LOG_SHARED
		if (shared_obj::try_found_shared_object(0) == NULL)
		{
			shared_obj_ptr_ = shared_obj::create_shared_object(0, dynamic_cast<logger_interface*>(this));
			shared_master_ = shared_obj_ptr_ ? true : false;
		}
#endif //LOG_SHARED

#if LOG_INI_CONFIGURATION
		log_ini_configurator::configure(configurator.get_ini_file_find_paths().c_str());
#endif //LOG_INI_CONFIGURATION

#if LOG_CONFIGURE_FROM_REGISTRY
		log_registry_configurator::configure(configurator.get_reg_config_path());
#endif //LOG_CONFIGURE_FROM_REGISTRY

#if LOG_CREATE_DIRECTORY

#	ifdef LOG_PLATFORM_WINDOWS
        SHCreateDirectoryExA(NULL, configurator.get_log_path().c_str(),NULL);
#	else //LOG_PLATFORM_WINDOWS
        mkpath(configurator.get_log_path().c_str(), 0777);
#	endif //LOG_PLATFORM_WINDOWS

#endif //LOG_CREATE_DIRECTORY

		if (configurator.get_verbose_level() == logger_verbose_mute)
			return;

		scroll_files(configurator.get_log_scroll_file_every_run());

#if LOG_MULTITHREADED
		LOG_MT_MUTEX_INIT(&mt_buffer_lock,NULL);

#	ifdef LOG_PLATFORM_WINDOWS
		write_event = CreateEvent(NULL, FALSE, TRUE, NULL);

		DWORD thread_id;
		log_thread_handle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)&log_thread_fn,this,0,&thread_id);LOG_PLATFORM_WINDOWS
#	else //LOG_PLATFORM_WINDOWS
		pthread_cond_init(&write_event, NULL);
		pthread_create(&log_thread_handle, NULL, (void*(*)(void*))&log_thread_fn, this);
#	endif //LOG_PLATFORM_WINDOWS

#endif //LOG_MULTITHREADED
		
		put_to_stream("\n");

#if LOG_USE_SYSTEMINFO
		if (configurator.get_need_sys_info())
			put_to_stream(query_system_info());
#endif //LOG_USE_SYSTEMINFO
	}

	virtual ~logger() 
	{
#if LOG_SHARED
		if (shared_master_ && shared_obj::free_shared_object(0))
		{
			shared_obj_ptr_ = NULL;
			shared_master_ = false;
#if LOG_CHECKED
			assert(shared_obj::try_found_shared_object(0) == NULL);
#endif //LOG_CHECKED
		}
#endif //LOG_SHARED

#if LOG_MULTITHREADED
		mt_terminating = 1;

#	ifdef LOG_PLATFORM_WINDOWS
		SetEvent(write_event);
		WaitForSingleObject(log_thread_handle, 10000);
		CloseHandle(write_event);
#	else //LOG_PLATFORM_WINDOWS
		pthread_cond_signal(&write_event);
		pthread_join(log_thread_handle,NULL);
		pthread_cond_destroy(&write_event);
#	endif //LOG_PLATFORM_WINDOWS

		LOG_MT_MUTEX_DESTROY(&mt_buffer_lock);

#endif //LOG_MULTITHREADED

#if !LOG_FLUSH_FILE_EVERY_WRITE && !LOG_MULTITHREADED
		stream.flush();
		stream.close();
#endif //LOG_FLUSH_FILE_EVERY_WRITE
	}

	void log_binary(int verbLevel, void* addr, const char* functionName, 
		const char* sourceFile, int lineNumber, const char* data, int len)
	{
		if (!is_message_enabled(verbLevel)) return;

		std::string moduleName = try_get_module_name_fast(addr);
		
		std::stringstream sstream;
		std::string header = log_process_macros(configurator.get_hdr_format(),verbLevel,lineNumber,sourceFile,functionName,moduleName.c_str());
		sstream << header;

		if (header.size())
			sstream << " " << std::endl;

		log_binary(sstream,data,len);
		put_to_stream(sstream.str());
	}

    void LOG_CDECL log(int verbLevel, void* addr, const char* functionName,
		const char* sourceFile, int lineNumber, const char* format, ...)
	{
		if (!is_message_enabled(verbLevel)) return;

		va_list arguments;
		va_start(arguments, format);

		log_args(verbLevel, addr, functionName, sourceFile, lineNumber, format, arguments);

		va_end(arguments);
	}

	void log_args(int verb_level, void* addr, const char* function_name, 
		const char* src_file, int line_num, const char* format, va_list arguments)
	{
		if (!is_message_enabled(verb_level)) return;

		std::string module_name = try_get_module_name_fast(addr);
		std::string str = log_process_macros(configurator.get_hdr_format(),verb_level,line_num,src_file,function_name,module_name.c_str());
		if (str.size())
			str += " ";

		std::string result;

#if LOG_PROCESS_MACRO_IN_LOG_TEXT
		std::string text = log_process_macroses_nocache(format,verb_level,line_num,src_file,function_name,module_name.c_str());
		format_arguments_list(result, text.c_str(), arguments);
#else //LOG_PROCESS_MACRO_IN_LOG_TEXT
		format_arguments_list(result, format, arguments);
#endif //LOG_PROCESS_MACRO_IN_LOG_TEXT

		put_to_stream(str + result + "\n");
	}

#if !LOG_USE_MODULEDEFINITION
	__inline std::string try_get_module_name_fast(void* ptr)
	{
        (void)ptr;
		return std::string();
	}
#endif //!LOG_USE_MODULEDEFINITION


#if LOG_USE_MODULEDEFINITION

#if LOG_USE_MODULES_CACHE

    typedef struct _cache_module_t
	{
		intptr_t base_addr;
		intptr_t end_addr;
		std::string name;
    } cache_module_t;

	std::vector<cache_module_t> cached_modules;

	bool is_module_found_in_cache(const char* name, intptr_t base_addr, intptr_t end_addr)
	{
		for (std::vector<cache_module_t>::iterator i=cached_modules.begin(); i!=cached_modules.end(); i++)
		{
			if (i->base_addr == base_addr && i->end_addr == end_addr && i->name == name)
				return true;
		}

		return false;
	}
#endif //LOG_USE_MODULES_CACHE

	std::string try_get_module_name_fast(void* ptr)
	{
#if LOG_USE_MODULES_CACHE
		intptr_t addr = (intptr_t)ptr;

		if (!addr)
			return std::string();

		for (size_t i=0; i<cached_modules.size(); i++)
		{
			if (cached_modules[i].base_addr <= addr && cached_modules[i].end_addr >= addr)
				return cached_modules[i].name;
		}

		std::list<module_entry_t> modules;
		if (module_definition::query_module_list(modules))
		{
			for (std::list<module_entry_t>::iterator i=modules.begin(); i!=modules.end(); i++)
			{
				if (!is_module_found_in_cache(i->moduleName.c_str(), i->baseAddress, i->baseAddress + i->size))
				{
					cache_module_t mod;

					mod.base_addr = i->baseAddress;
					mod.end_addr = i->size + i->baseAddress;
					mod.name = i->moduleName;
					cached_modules.push_back(mod);
				}
			}
		}
#endif // LOG_USE_MODULES_CACHE

		return logging::module_definition::module_name_by_addr(ptr);
	}

	void log_modules(int verb_level, void* addr, const char* function_name, 
		const char* sourceFile, int lineNumber)
	{
		if (!is_message_enabled(verb_level)) return;

		std::string module_name = try_get_module_name_fast(addr);

		std::stringstream sstream;

		std::string header = log_process_macros(configurator.get_hdr_format(),verb_level,lineNumber,sourceFile,function_name,module_name.c_str());
		sstream << header;
		
		if (header.size())
			sstream << std::endl;

		std::list<module_entry_t> modules;
		if (!module_definition::query_module_list(modules))
			return;

		sstream << "Base\tSize\tModule name\tImage name\tVersion info" << std::endl;
		sstream << "------------------------------------------------" << std::endl;

		for (std::list<module_entry_t>::iterator i=modules.begin(); i!=modules.end(); i++)
		{
#ifdef LOG_PLATFORM_64BIT
            sstream << stringformat("0x%.8X%.8X", (uint32_t)((*i).baseAddress >> 32), (*i).baseAddress) << "\t";
#endif //LOG_PLATFORM_64BIT

#ifdef LOG_PLATFORM_32BIT
			sstream << stringformat("0x%.8X",(*i).baseAddress) << "\t";
#endif //LOG_PLATFORM_32BIT

			sstream << (*i).size << "\t" << (*i).moduleName.c_str() << "\t";
			sstream << (*i).imageName.c_str() << "\t";

			if ((*i).lang_id)
				sstream << "LangId:" << stringformat("%.4X", (*i).lang_id) << "\t";

			if ((*i).file_version.size())
				sstream << "FileVersion:" << "\"" << (*i).file_version << "\"" << "\t";

			if ((*i).product_version.size())
				sstream << "ProductVersion:" <<"\"" << (*i).product_version << "\"" << "\t";

			if ((*i).company_name.size())
				sstream << "CompanyName:" << "\"" << (*i).company_name << "\"" << "\t";

			if ((*i).file_description.size())
				sstream << "FileDescription:" << "\"" << (*i).file_description << "\"" << "\t";

			sstream << std::endl;
		}

		put_to_stream(sstream.str());
	}
#endif //LOG_USE_MODULEDEFINITION


#if LOG_AUTO_DEBUGGING

#ifdef LOG_PLATFORM_WINDOWS

	LONG WINAPI exception_catch_stack_trace_filter(EXCEPTION_POINTERS* pExp, DWORD dwExpCode, char** stack_trace)
	{
		std::string trace = runtime_debugging::get_stack_trace_string(pExp->ContextRecord, 2);
		
		*stack_trace = (char*) malloc(trace.size() + 1);
		memset(*stack_trace, 0, trace.size() + 1);
		memcpy(*stack_trace, trace.c_str(), trace.size());

		return EXCEPTION_EXECUTE_HANDLER;
	}


#ifdef LOG_COMPILER_MSVC
#pragma optimize ( "", off )
#endif //LOG_COMPILER_MSVC

	void get_current_stack_trace_string(char** stack_trace)
	{
		*stack_trace = NULL;
		
		__try
		{
			int a = 10;
			int b = 0;
			a = a / b;
		}
		__except (exception_catch_stack_trace_filter(GetExceptionInformation(), GetExceptionCode(), stack_trace))
		{}
	}

#ifdef LOG_COMPILER_MSVC
#pragma optimize ( "", on )
#endif //LOG_COMPILER_MSVC


#else //LOG_PLATFORM_WINDOWS

    std::string get_current_stack_trace_string()
    {
        void *bt[1024];
        int bt_size = backtrace(bt, 1024);
        return runtime_debugging::get_stack_trace_string(bt, bt_size, 1);
    }
#endif //LOG_PLATFORM_WINDOWS

	void log_stack_trace(int verb_level, void* addr, const char* function_name, 
		const char* src_file, int lineNumber)
	{
		if (!is_message_enabled(verb_level)) return;

#ifdef LOG_PLATFORM_WINDOWS
        char* stack_trace = NULL;
        get_current_stack_trace_string(&stack_trace);
		std::string stack = stack_trace; 
        free(stack_trace);
#else //LOG_PLATFORM_WINDOWS
        std::string stack = get_current_stack_trace_string();
#endif //LOG_PLATFORM_WINDOWS

		std::string module_name = try_get_module_name_fast(addr);

		std::stringstream sstream;
		std::string header = log_process_macros(configurator.get_hdr_format(),verb_level,lineNumber,src_file,function_name,module_name.c_str());
		sstream << header;
		if (header.size())
			sstream << " ";

		sstream << "Stack trace:" << std::endl << stack << std::endl;

		put_to_stream(sstream.str());
	}
#endif //LOG_AUTO_DEBUGGING


	void log_exception(int verbLevel, void* addr, const char* function_name, 
		const char* src_file, int line_num, const char* userMessage)
	{
		if (!is_message_enabled(verbLevel)) return;

		std::string module_name = try_get_module_name_fast(addr);

		std::stringstream sstream;
		std::string header = log_process_macros(configurator.get_hdr_format(),verbLevel,line_num,src_file,function_name,module_name.c_str());
		sstream << header;

		if (header.size())
			sstream << " ";

		sstream << "*** Exception occured at " << src_file << " (line " << line_num << ")" << std::endl;
		sstream << userMessage << std::endl;

		put_to_stream(sstream.str());
	}

	void log_exception(int verbLevel, void* addr, const char* function_name, 
		const char* src_file, int line_num, std::exception* e)
	{
		if (!is_message_enabled(verbLevel)) return;

		std::string message;
		if (e != NULL)
		{
#if LOG_RTTI_ENABLED
			message += std::string("Exception type: ") + std::string(typeid(*e).name()) + "\n";
#endif //LOG_RTTI_ENABLED

			if (e->what())
				message += std::string(e->what()) + "\n";
		}
		log_exception(verbLevel,addr,function_name,src_file,line_num,message.c_str());
	}

private:

#if LOG_USE_MACRO_HEADER_CACHE
	struct log_macro_cache_t
	{
		std::string processed_cached_hdr_full;
		std::string processed_cached_hdr_partial;
		struct tm processed_cached_tm;
		std::string processed_cached_src_file;
		std::string processed_cached_function_name;
		std::string processed_cached_module_name;
		std::string processed_cached_hdr_format;
		int processed_cached_global_verb_level;
		int processed_cached_verb_level;
		int processed_cached_line_num;
		int processed_cached_millitm;
		unsigned long processed_cached_pid;
		unsigned long processed_cached_tid;


#if LOG_MULTITHREADED
		LOG_MT_MUTEX mt_cache_lock;
#endif //LOG_MULTITHREADED

		log_macro_cache_t()
		{
#if LOG_MULTITHREADED
			LOG_MT_MUTEX_INIT(&mt_cache_lock,NULL);
#endif //LOG_MULTITHREADED
		}

		virtual ~log_macro_cache_t()
		{
#if LOG_MULTITHREADED
			LOG_MT_MUTEX_DESTROY(&mt_cache_lock);
#endif //LOG_MULTITHREADED
		}

		inline void lock()
		{
#if LOG_MULTITHREADED
			LOG_MT_MUTEX_LOCK(&mt_cache_lock);
#endif //LOG_MULTITHREADED
		}

		inline void unlock()
		{
#if LOG_MULTITHREADED
			LOG_MT_MUTEX_UNLOCK(&mt_cache_lock);
#endif //LOG_MULTITHREADED
		}
	};

	log_macro_cache_t cache;
#endif //LOG_USE_MACRO_HEADER_CACHE


	std::string log_process_macroses_nocache(std::string format, 
									int verbose, 
									int line_num, 
									const char* src_file,	
									const char* function_name, 
									const char* module_name)
	{
#if LOG_USE_MACRO_HEADER_CACHE
		std::string result = log_process_macros_setlen(format, module_name);
		result = log_process_macros_often(result, verbose, line_num, src_file, function_name);
		return result;
#else //LOG_USE_MACRO_HEADER_CACHE
		return log_process_macros(result, verbose, line_num, src_file, function_name, module_name);
#endif //LOG_USE_MACRO_HEADER_CACHE
	}
	

	static unsigned long get_thread_id()
	{
		unsigned long tid = 0;

#ifdef LOG_PLATFORM_WINDOWS
		tid = GetCurrentThreadId();
#else //LOG_PLATFORM_WINDOWS

#	ifdef gettid
		tid = (unsigned long) gettid();
#	else //gettid
#		if defined(SYS_gettid) && defined(LOG_HAVE_SYS_SYSCALL_H)
			tid = (unsigned long)syscall(SYS_gettid);
#		else //defined(SYS_gettid) && defined(LOG_HAVE_SYS_SYSCALL_H)

#			ifdef LOG_MULTITHREADED		
				tid = (unsigned long) pthread_self();
#			endif //LOG_MULTITHREADED
#		endif //defined(SYS_gettid) && defined(LOG_HAVE_SYS_SYSCALL_H)

#	endif //gettid

#endif //LOG_PLATFORM_WINDOWS
		return tid;
	}


	std::string log_process_macros(std::string format, 
									int verbose, 
									int line_num, 
									const char* src_file,	
									const char* function_name, 
									const char* module_name)
	{
		std::string result;

#if LOG_USE_MACRO_HEADER_CACHE
		bool recreate = false;

		cache.lock();

		if (configurator.get_hdr_format() != cache.processed_cached_hdr_format
			|| configurator.get_verbose_level() != cache.processed_cached_global_verb_level)
		{
			recreate = true;

			cache.processed_cached_hdr_format = configurator.get_hdr_format();
			cache.processed_cached_global_verb_level = configurator.get_verbose_level();
		}

		unsigned long pid=0, tid = get_thread_id();
#ifdef LOG_PLATFORM_WINDOWS
		pid = GetCurrentProcessId();
#else //LOG_PLATFORM_WINDOWS

#	ifdef LOG_HAVE_UNISTD_H
		pid = (unsigned long)getpid();
#	endif //LOG_HAVE_UNISTD_H

#endif //LOG_PLATFORM_WINDOWS

        int millisec;
        struct tm newtime = utils::get_time(millisec);

		if (!recreate  
			&& cache.processed_cached_hdr_partial.size()
			&& pid == cache.processed_cached_pid
			&& newtime.tm_year == cache.processed_cached_tm.tm_year 
			&& newtime.tm_mon == cache.processed_cached_tm.tm_mon
			&& newtime.tm_mday == cache.processed_cached_tm.tm_mday
			&& newtime.tm_hour == cache.processed_cached_tm.tm_hour
			&& newtime.tm_min == cache.processed_cached_tm.tm_min
			&& !strcmp(module_name, cache.processed_cached_module_name.c_str()) )
		{
			if (tid == cache.processed_cached_tid
				&& newtime.tm_sec == cache.processed_cached_tm.tm_sec
                && millisec == cache.processed_cached_millitm
				&& line_num == cache.processed_cached_line_num
				&& !strcmp(cache.processed_cached_function_name.c_str(), function_name) 
				&& verbose == cache.processed_cached_verb_level)
			{
				result = cache.processed_cached_hdr_full;
			}
			else
			{
				result = log_process_macros_often(cache.processed_cached_hdr_partial, verbose, line_num, src_file, function_name);
			}

            cache.processed_cached_millitm = millisec;
			cache.processed_cached_hdr_full = result;
			cache.processed_cached_src_file = src_file;
			cache.processed_cached_function_name = function_name;
			cache.processed_cached_line_num = line_num;
			cache.processed_cached_tm = newtime;
			cache.processed_cached_verb_level = verbose;
			cache.processed_cached_pid = pid;
			cache.processed_cached_tid = tid;

			cache.unlock();
			return result;
		}

#endif //LOG_USE_MACRO_HEADER_CACHE

		result = log_process_macros_setlen(format, module_name);

#if LOG_USE_MACRO_HEADER_CACHE
		cache.processed_cached_hdr_partial = result;
#endif //LOG_USE_MACRO_HEADER_CACHE

		result = log_process_macros_often(result, verbose, line_num, src_file, function_name);

#if LOG_USE_MACRO_HEADER_CACHE
		cache.processed_cached_hdr_full = result;
		cache.processed_cached_module_name = module_name;
        cache.processed_cached_millitm = millisec;
		cache.processed_cached_hdr_full = result;
		cache.processed_cached_src_file = src_file;
		cache.processed_cached_function_name = function_name;
		cache.processed_cached_line_num = line_num;
		cache.processed_cached_tm = newtime;
		cache.processed_cached_verb_level = verbose;
		cache.processed_cached_pid = pid;
		cache.processed_cached_tid = tid;

		cache.unlock();
#endif //LOG_USE_MACRO_HEADER_CACHE

		return result;
	}

	std::string log_process_macros_setlen(std::string format, 
									const char* module_name)
	{
		const char *format_str = format.c_str();

		bool macro_yyyy = contains(format_str,"$(yyyy)");
		bool macro_yy = contains(format_str,"$(yy)");
		bool macro_MM = contains(format_str,"$(MM)");
		bool macro_M = contains(format_str,"$(M)");
		bool macro_dd = contains(format_str,"$(dd)");
		bool macro_d = contains(format_str,"$(d)");
		bool macro_hh = contains(format_str,"$(hh)");
		bool macro_h = contains(format_str,"$(h)");
		bool macro_mm = contains(format_str,"$(mm)");
		bool macro_m = contains(format_str,"$(m)");

		bool macro_module = contains(format_str,"$(module)");
		bool macro_MODULE = contains(format_str,"$(MODULE)");

		bool macro_PID = contains(format_str,"$(PID)");

		if (macro_yyyy || macro_yy || macro_MM || macro_M || macro_dd || macro_d || macro_hh || macro_h || macro_mm || macro_m || macro_PID)
		{
            int millisec;
            struct tm newtime = utils::get_time(millisec);
		
			if (macro_yyyy)	format = replace(format,"$(yyyy)",stringformat("%.4d",newtime.tm_year + 1900));
			if (macro_yy)   format = replace(format,"$(yy)",stringformat("%.2d",newtime.tm_year - 100));
			if (macro_M)    format = replace(format,"$(M)",stringformat("%d",newtime.tm_mon+1));
			if (macro_MM)   format = replace(format,"$(MM)",stringformat("%.2d",newtime.tm_mon+1));
			if (macro_d)    format = replace(format,"$(d)",stringformat("%d",newtime.tm_mday));
			if (macro_dd)   format = replace(format,"$(dd)",stringformat("%.2d",newtime.tm_mday));
			if (macro_h)    format = replace(format,"$(h)",stringformat("%d",newtime.tm_hour));
			if (macro_hh)   format = replace(format,"$(hh)",stringformat("%.2d",newtime.tm_hour));
			if (macro_m)    format = replace(format,"$(m)",stringformat("%d",newtime.tm_min));
			if (macro_mm)   format = replace(format,"$(mm)",stringformat("%.2d",newtime.tm_min));
			
			if (macro_PID)  
			{
				unsigned long pid = 0;

#ifdef LOG_PLATFORM_WINDOWS
				pid = GetCurrentProcessId();
#else //LOG_PLATFORM_WINDOWS

#	ifdef LOG_HAVE_UNISTD_H
				pid = (unsigned long)getpid();
#	endif //LOG_HAVE_UNISTD_H

#endif //LOG_PLATFORM_WINDOWS
				format = replace(format,"$(PID)", stringformat("%d", pid));
			}
		}


		if ((macro_module || macro_MODULE) && strlen(module_name))
		{
			if (macro_MODULE) format = replace(format,"$(MODULE)",module_name);

			if (macro_module)
			{
				std::string resultName = module_name;
                size_t position = resultName.find_last_of("\\/");
				if (position == resultName.npos)
					position = 0;
				else
					position++;

				format = replace(format,"$(module)",resultName.substr(position));
			}
		}
		return format;
	}
	

	std::string log_process_macros_often(std::string format, 
									int verbose, 
									int line_num, 
									const char* src_file,	
									const char* function_name)
	{
		const char *format_str = format.c_str();

		bool macro_ss = contains(format_str,"$(ss)");
		bool macro_s = contains(format_str,"$(s)");
		bool macro_ttt = contains(format_str,"$(ttt)");
		bool macro_t =contains(format_str,"$(t)");

		bool macro_srcfile = contains(format_str,"$(srcfile)");
		bool macro_line = contains(format_str,"$(line)");
		bool macro_function = contains(format_str,"$(function)");

		bool macro_V = contains(format_str,"$(V)");
		bool macro_v = contains(format_str,"$(v)");

		bool macro_tid = contains(format_str, "$(TID)");
		

		if (macro_ss || macro_s || macro_ttt || macro_t)
		{
            int millisec;
            struct tm newtime = utils::get_time(millisec);
		
			if (macro_s)    format = replace(format,"$(s)",stringformat("%d",newtime.tm_sec));
			if (macro_ss)   format = replace(format,"$(ss)",stringformat("%.2d",newtime.tm_sec));
            if (macro_t)    format = replace(format,"$(t)",stringformat("%d",millisec));
            if (macro_ttt)  format = replace(format,"$(ttt)",stringformat("%.3d",millisec));
		}

		if (macro_V) format = replace(format,"$(V)",get_verbose_string(verbose));
		if (macro_v) format = replace(format,"$(v)",stringformat("%d",verbose));
	
		if (macro_srcfile && strlen(src_file))
			format = replace(format,"$(srcfile)",src_file);
		
		if (macro_line && line_num >= 0)
			format = replace(format,"$(line)",stringformat("%d",line_num));

		if (macro_function && strlen(function_name))
			format = replace(format,"$(function)",function_name);

		if (macro_tid)
		{
			unsigned long tid = get_thread_id();
			format = replace(format,"$(TID)", stringformat("%d", tid));
		}

		return format;
	}

	__inline bool is_message_enabled(int verb_level) const
	{
		return (configurator.get_verbose_level() & verb_level) ? true : false;
	}

	void log_binary(std::ostream& stream, const char* data, int len)
	{
		const int output_byte_width = 16;
		int current_element = 0;

		while(current_element < len)
		{
			int line_start_element = current_element;
			stream << stringformat("%.4X: ",line_start_element);

			for (int i=0; i<output_byte_width; i++)
			{
				if (current_element < len)
					stream << stringformat("%.2X ",static_cast<unsigned char>(data[current_element++]));
				else
					stream << "   ";
			}

			stream << "|";
			for (int j=line_start_element; j<line_start_element+output_byte_width; j++)
			{
				if (j >= len) break;
				stream << (static_cast<unsigned char>(data[j]) >= '!' ? data[j] : '.');
			}

			stream << std::endl;
		}
	}

	static const char* get_verbose_string(int verbose_level)
	{
		switch(verbose_level)
		{
			case logger_verbose_mute:  return "MUTE";
			case logger_verbose_fatal: return "FATAL";
			case logger_verbose_error: return "ERROR";
			case logger_verbose_info:  return "INFO";
			case logger_verbose_warning: return "WARNING";
			default:
			case logger_verbose_debug: return "DEBUG";
		}
	}
};

extern singleton<logger_interface, logger> _logger;

//////////////////////////////////////////////////////////////
#if LOG_UNHANDLED_EXCEPTIONS

#if defined(LOG_PLATFORM_WINDOWS)
class unhandled_exceptions_processor
{
private:
	static BOOL CALLBACK minidump_callback(PVOID param, const PMINIDUMP_CALLBACK_INPUT input, PMINIDUMP_CALLBACK_OUTPUT output) 
	{
 		if(input == 0 || output ==0) 
			return FALSE; 

		switch(input->CallbackType)
		{
			case IncludeModuleCallback:
			case IncludeThreadCallback:
			case ModuleCallback:
			case ThreadCallback:
			case ThreadExCallback:
			case MemoryCallback: 
				return TRUE;  

			default:
 			case CancelCallback: return FALSE;
		}
	}

	static void create_minidump(EXCEPTION_POINTERS* ex_ptrs, const std::string& minidump_file_path) 
	{
		HANDLE file_handle = CreateFileA(minidump_file_path.c_str(), GENERIC_READ | GENERIC_WRITE,
			0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (file_handle == NULL || file_handle == INVALID_HANDLE_VALUE)
		{
			LOG_ERROR("Cannot create minidump file: %s, error: %u",minidump_file_path.c_str(),GetLastError());
			return;
		}

		MINIDUMP_EXCEPTION_INFORMATION mdei; 

		mdei.ThreadId           = GetCurrentThreadId(); 
		mdei.ExceptionPointers  = ex_ptrs; 
		mdei.ClientPointers     = FALSE; 

		MINIDUMP_CALLBACK_INFORMATION mci; 

		mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)unhandled_exceptions_processor::minidump_callback; 
		mci.CallbackParam       = 0; 

		MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)(MiniDumpNormal |
									MiniDumpWithHandleData |
									MiniDumpWithThreadInfo |
									MiniDumpWithUnloadedModules);

		BOOL rv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), 
		file_handle, mdt, (ex_ptrs != 0) ? &mdei : 0, 0, &mci ); 

		if(!rv) 
		{
			LOG_ERROR("MiniDumpWriteDump failed. Error: %u", GetLastError()); 
		}
		else 
		{
			LOG_INFO("Minidump created: %s",minidump_file_path.c_str()); 
		}

		CloseHandle(file_handle); 
		return;
	}

public:
	static LPTOP_LEVEL_EXCEPTION_FILTER prev_exception_filter;

	static long __stdcall crash_handler_exception_filter(EXCEPTION_POINTERS* ex_ptrs)
	{
	   std::string message = stringformat("*** Unhandled Exception!\n"
			"   Exception code: 0x%8.8X (%s)\n"
			"   Exception flags: %d\n"
			"   Exception address: 0x%8.8X\n",
			ex_ptrs->ExceptionRecord->ExceptionCode, 
			runtime_debugging::get_system_expection_code_text(ex_ptrs->ExceptionRecord->ExceptionCode),
			ex_ptrs->ExceptionRecord->ExceptionFlags,
			ex_ptrs->ExceptionRecord->ExceptionAddress);

		LOG_FATAL(message.c_str());

		LOG_MODULES_FATAL;

	#if LOG_AUTO_DEBUGGING
		LOG_FATAL("Stack trace:\n%s", runtime_debugging::get_stack_trace_string(ex_ptrs->ContextRecord).c_str());
	#endif //LOG_AUTO_DEBUGGING

		int millisec;
		struct tm newtime = utils::get_time(millisec);

		std::string dump_name = stringformat("%s\\%s__%.4d_%.2d_%.2d__%.2d_%.2d_%.2d.dmp",
			configurator.get_log_path().c_str(),
			configurator.get_log_file_name().c_str(),
			newtime.tm_year+1900,newtime.tm_mon+1,newtime.tm_mday,newtime.tm_hour,
			newtime.tm_min,newtime.tm_sec);

		create_minidump(ex_ptrs, dump_name);

		if (prev_exception_filter)
			return prev_exception_filter(ex_ptrs);

#if LOG_RELEASE_ON_APP_CRASH
		_logger.release();
#endif //LOG_RELEASE_ON_APP_CRASH

#if LOG_SHOW_MESSAGE_ON_FATAL_CRASH
		FatalAppExitA(-1,message.c_str());
		return -1;
#else //LOG_SHOW_MESSAGE_ON_FATAL_CRASH
		return EXCEPTION_CONTINUE_SEARCH;
#endif //LOG_SHOW_MESSAGE_ON_FATAL_CRASH
	}
};
#else // defined(LOG_PLATFORM_WINDOWS)

static void crash_handler(int sig, siginfo_t *info, void *secret)
{
    void *trace[1024];
    ucontext_t *uc = (ucontext_t *)secret;

#ifdef LOG_PLATFORM_64BIT
    void* pc = (void*)uc->uc_mcontext.gregs[REG_RIP];
#endif //LOG_PLATFORM_64BIT

#ifdef LOG_PLATFORM_32BIT
    void* pc = (void*)uc->uc_mcontext.gregs[REG_EIP];
#endif //LOG_PLATFORM_32BIT

    std::string message = stringformat("*** Got signal %d, faulty address %p, from %p",sig, info->si_addr, pc);
    LOG_FATAL(message.c_str());
    LOG_MODULES_FATAL;

    int trace_size = backtrace(trace, 1024);
    trace[1] = pc;

    std::string result = "\n" + runtime_debugging::get_stack_trace_string(trace, trace_size, 0);

    LOG_FATAL("*** STACKTRACE ***");
    LOG_FATAL(result.c_str());
    LOG_FATAL("*** END STACKTRACE ***");

#if LOG_RELEASE_ON_APP_CRASH
    _logger.release();
#endif //LOG_RELEASE_ON_APP_CRASH

#if LOG_SHOW_MESSAGE_ON_FATAL_CRASH
    printf("%s\n", message.c_str());
#endif //LOG_SHOW_MESSAGE_ON_FATAL_CRASH

    exit(-1);
}

#endif // defined(LOG_PLATFORM_WINDOWS)


static void init_unhandled_exceptions_handler()
{
#ifdef LOG_PLATFORM_WINDOWS
    unhandled_exceptions_processor::prev_exception_filter =
    SetUnhandledExceptionFilter(unhandled_exceptions_processor::crash_handler_exception_filter);
#else //LOG_PLATFORM_WINDOWS

    struct sigaction sa;

    sa.sa_sigaction = crash_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGSEGV, &sa, NULL);
#endif //LOG_PLATFORM_WINDOWS
}

#endif //LOG_UNHANDLED_EXCEPTIONS

}; // namespace logging


///////////////   C tail  ///////////////////////

#else //defined(__cplusplus) && (!LOG_USE_DLL || defined(LOG_THIS_IS_DLL))

#if defined (__cplusplus)
extern "C" {
#endif //defined(__cplusplus)

#	define LOG_INFO(...)    __c_logger_log(logger_verbose_info, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,__VA_ARGS__)
#	define LOG_DEBUG(...)   __c_logger_log(logger_verbose_debug, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,__VA_ARGS__)
#	define LOG_WARNING(...) __c_logger_log(logger_verbose_warning, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,__VA_ARGS__)
#	define LOG_ERROR(...)   __c_logger_log(logger_verbose_error, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,__VA_ARGS__)
#	define LOG_FATAL(...)   __c_logger_log(logger_verbose_fatal, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,__VA_ARGS__)

#	define LOG_BINARY_INFO(p,stack_frame)    __c_logger_log_binary(logger_verbose_info, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,p,stack_frame)
#	define LOG_BINARY_DEBUG(p,stack_frame)   __c_logger_log_binary(logger_verbose_debug, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,p,stack_frame)
#	define LOG_BINARY_WARNING(p,stack_frame) __c_logger_log_binary(logger_verbose_warning, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,p,stack_frame)
#	define LOG_BINARY_ERROR(p,stack_frame)   __c_logger_log_binary(logger_verbose_error, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,p,stack_frame)
#	define LOG_BINARY_FATAL(p,stack_frame)   __c_logger_log_binary(logger_verbose_fatal, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__,p,stack_frame)

#if LOG_USE_DLL
extern void (LOG_CDECL *__c_logger_log_modules)(int verbose_level, void* caller_addr, const char* function, const char* file, int line);
#endif //LOG_USE_DLL

#if LOG_USE_MODULEDEFINITION

#	define LOG_MODULES_DEBUG       __c_logger_log_modules(logger_verbose_debug, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__)
#	define LOG_MODULES_INFO        __c_logger_log_modules(logger_verbose_info, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__)
#	define LOG_MODULES_WARNING     __c_logger_log_modules(logger_verbose_warning, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__)
#	define LOG_MODULES_ERROR       __c_logger_log_modules(logger_verbose_error, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__)
#	define LOG_MODULES_FATAL       __c_logger_log_modules(logger_verbose_fatal, LOG_GET_CALLER_ADDR,__FUNCTION__,__FILE__,__LINE__)

#if !LOG_USE_DLL
void __c_logger_log_modules(int verbose_level, void* caller_addr, const char* function, const char* file, int line);
#endif //!LOG_USE_DLL

#else //LOG_USE_MODULEDEFINITION

#	define LOG_MODULES_DEBUG
#	define LOG_MODULES_INFO
#	define LOG_MODULES_WARNING
#	define LOG_MODULES_ERROR
#	define LOG_MODULES_FATAL

#endif //LOG_USE_MODULEDEFINITION

#if LOG_USE_DLL
extern void (LOG_CDECL* __c_logger_log_stack_trace)(int verbose_level, void* caller_addr, const char* function, const char* file, int line);
#endif //LOG_USE_DLL

#if LOG_AUTO_DEBUGGING

#	define LOG_STACKTRACE_DEBUG    __c_logger_log_stack_trace(logger_verbose_debug, logging_get_caller_address(),__FUNCTION__,__FILE__,__LINE__)
#	define LOG_STACKTRACE_INFO     __c_logger_log_stack_trace(logger_verbose_info, logging_get_caller_address(),__FUNCTION__,__FILE__,__LINE__)
#	define LOG_STACKTRACE_WARNING  __c_logger_log_stack_trace(logger_verbose_warning, logging_get_caller_address(),__FUNCTION__,__FILE__,__LINE__)
#	define LOG_STACKTRACE_ERROR    __c_logger_log_stack_trace(logger_verbose_error, logging_get_caller_address(),__FUNCTION__,__FILE__,__LINE__)
#	define LOG_STACKTRACE_FATAL    __c_logger_log_stack_trace(logger_verbose_fatal, logging_get_caller_address(),__FUNCTION__,__FILE__,__LINE__)

#if !LOG_USE_DLL
void __c_logger_log_stack_trace(int verbose_level, void* caller_addr, const char* function, const char* file, int line);
#endif //!LOG_USE_DLL

#else //LOG_AUTO_DEBUGGING

#	define LOG_STACKTRACE_DEBUG  
#	define LOG_STACKTRACE_INFO   
#	define LOG_STACKTRACE_WARNING
#	define LOG_STACKTRACE_ERROR  
#	define LOG_STACKTRACE_FATAL  

#endif //LOG_AUTO_DEBUGGING

#if LOG_USE_DLL
extern void (LOG_CDECL *__c_logger_log)(int verbose_level, void* caller_addr, const char* function, const char* file, int line, const char* format, ...);
extern void (LOG_CDECL *__c_logger_log_args)(int verbose_level, void* caller_addr, const char* function, const char* file, int line, const char* format, va_list args);
extern void (LOG_CDECL *__c_logger_log_binary)(int verbose_level, void* caller_addr, const char* function, const char* file, int line, const char* data, int len);
#else //LOG_USE_DLL
void __c_logger_log(int verbose_level, void* caller_addr, const char* function, const char* file, int line, const char* format, ...);
void __c_logger_log_args(int verbose_level, void* caller_addr, const char* function, const char* file, int line, const char* format, va_list args);
void __c_logger_log_binary(int verbose_level, void* caller_addr, const char* function, const char* file, int line, const char* data, int len);
#endif //LOG_USE_DLL

#if defined (__cplusplus)
}
#endif //defined(__cplusplus)

#endif //defined(__cplusplus) && (!LOG_USE_DLL || defined(LOG_THIS_IS_DLL))

#endif // LOG_ENABLED

#endif //__LOGGER_HEADER
