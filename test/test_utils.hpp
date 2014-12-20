// mapnik
#include "mapnik3x_compatibility.hpp"
#include <mapnik/memory_datasource.hpp>

// boost
#include MAPNIK_SHARED_INCLUDE
#include MAPNIK_MAKE_SHARED_INCLUDE

#include <string>

MAPNIK_SHARED_PTR<mapnik::memory_datasource> build_ds(double x,double y, bool second=false);
