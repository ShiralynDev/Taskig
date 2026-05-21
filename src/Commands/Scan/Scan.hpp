#pragma once

#include "../../CacheUtils/CacheUtils.hpp"
#include "../../TaskScan/TaskScan.hpp"

namespace Commands {

    int scanCommand(int argc, char* argv[], CacheUtils::Cache& cache, Scan::TaskScan& taskScan);

}