#ifndef __MAPNIK_VTILE_COMPARE_IMAGES_HPP__
#define __MAPNIK_VTILE_COMPARE_IMAGES_HPP__ 

#include "mapnik3x_compatibility.hpp"
#include MAPNIK_SHARED_INCLUDE
#include MAPNIK_MAKE_SHARED_INCLUDE
#include <mapnik/graphics.hpp>

using namespace mapnik;

namespace testing {

    unsigned compare_images(MAPNIK_IMAGE_RGBA const& src1,
                            MAPNIK_IMAGE_RGBA const& src2,
                            int threshold=16,
                            bool alpha=true);

    unsigned compare_images(std::string const& src_fn,
                            std::string const& dest_fn,
                            int threshold=16,
                            bool alpha=true);

    unsigned compare_images(MAPNIK_IMAGE_RGBA const& src1,
                            std::string const& filepath,
                            int threshold=16,
                            bool alpha=true);

}

#endif // __MAPNIK_VTILE_COMPARE_IMAGES_HPP__
