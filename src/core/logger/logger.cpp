#include "logger.h"
#include "pch.h"

std::shared_ptr<spdlog::logger> logger::l;

std::shared_ptr<spdlog::logger> logger::log()
{
	if (!l)
	{
		l = spdlog::default_logger();
	}

	return l;
}

void logger::init_logger(const std::string& app_name, spdlog::level::level_enum level, const std::string& pattern)
{
	l = spdlog::stdout_color_mt(app_name);
	l->set_level(level);
	l->set_pattern(pattern);
}
