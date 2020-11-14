
#ifndef LOGGER_REGISTER_BUILTIN_PLUGIN
#define LOGGER_REGISTER_BUILTIN_PLUGIN

#include "logger_config.h"
#include "logger_interfaces.h"
#include <sstream>

#include "logger_ini_config_plugin.h"
#include "logger_scroll_file_output_plugin.h"
#include "logger_binary_command_plugin.h"
#include "logger_objmon_command_plugin.h"
#include "logger_colored_console_output_plugin.h"

#if LOG_AUTO_DEBUGGING
#include "logger_crashhandler_command_plugin.h"
#endif /*LOG_AUTO_DEBUGGING*/

#if LOG_USE_MODULEDEFINITION
#include "logger_modules_command_plugin.h"
#endif /*LOG_USE_MODULEDEFINITION*/

#if LOG_AUTO_DEBUGGING
#include "logger_stacktrace_command_plugin.h"
#endif /*LOG_AUTO_DEBUGGING*/

#ifdef LOG_PLATFORM_WINDOWS
#include "logger_win_config_macro_plugin.h"
#include "logger_win_registry_config_plugin.h"
#endif /*LOG_PLATFORM_WINDOWS*/

namespace logging {

class logger_register_builtin_plugin : public logger_plugin_interface {
public:
  logger_register_builtin_plugin(const char* name = NULL) : plugin_name_(name ? name : "") {
    plugin_factories_.push_back(new logger_scroll_file_output_plugin_factory());
    plugin_factories_.push_back(new logger_binary_command_plugin_factory());
    plugin_factories_.push_back(new logger_ini_config_plugin_factory());


    plugin_factories_.push_back(new logger_objmon_command_plugin_factory());
    plugin_factories_.push_back(new logger_colored_console_output_plugin_factory());

#if LOG_USE_MODULEDEFINITION
    plugin_factories_.push_back(new logger_modules_command_plugin_factory());
#endif /*LOG_USE_MODULEDEFINITION*/

#if LOG_AUTO_DEBUGGING
    plugin_factories_.push_back(new logger_crashhandler_command_plugin_factory());
    plugin_factories_.push_back(new logger_stacktrace_command_plugin_factory());
#endif /*LOG_AUTO_DEBUGGING*/

#ifdef LOG_PLATFORM_WINDOWS
    plugin_factories_.push_back(new logger_win_config_macro_plugin_factory());
    plugin_factories_.push_back(new logger_win_registry_config_plugin_factory());
#endif /*LOG_PLATFORM_WINDOWS*/
  }

  virtual ~logger_register_builtin_plugin() LOG_METHOD_OVERRIDE {
    for (size_t i = 0; i < plugin_factories_.size(); i++) {
      delete plugin_factories_[i];
    }
    plugin_factories_.clear();
  }

  const char* type() const LOG_METHOD_OVERRIDE { return "builtin"; }

  const char* name() const LOG_METHOD_OVERRIDE { return plugin_name_.c_str(); }

  virtual bool attach(logger_interface* logger) LOG_METHOD_OVERRIDE {
    for (size_t i = 0; i < plugin_factories_.size(); i++) {
      logger->register_plugin_factory(plugin_factories_[i]);
    }

    return true; 
  }

  virtual void detach(logger_interface* logger) LOG_METHOD_OVERRIDE {
    for (size_t i = 0; i < plugin_factories_.size(); i++) {
      logger->unregister_plugin_factory(plugin_factories_[i]);
    }
  }

  void config_updated(const logging::cfg::KeyValueTypeList& config) LOG_METHOD_OVERRIDE {}

private:
  std::string plugin_name_;
  std::vector<logger_plugin_factory_interface*> plugin_factories_;
};

class logger_register_builtin_plugin_factory : public logger_plugin_default_factory<logger_register_builtin_plugin> {
public:
  logger_register_builtin_plugin_factory()
    : logger_plugin_default_factory<logger_register_builtin_plugin>("builtin", kLogPluginTypeBehavior) {}
  virtual ~logger_register_builtin_plugin_factory() LOG_METHOD_OVERRIDE {}
};

}//namespace logging

#endif /*LOGGER_REGISTER_BUILTIN_PLUGIN*/
