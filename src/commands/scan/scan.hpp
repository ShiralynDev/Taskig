#pragma once

#include "../../cacheUtils/cacheUtils.hpp"
#include "../../taskScan/taskScan.hpp"

namespace Commands {

    int scanCommand(int argc, char* argv[], CacheUtils::Cache& cache, Scan::TaskScan& taskScan);

}