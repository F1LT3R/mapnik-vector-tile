#include "test_utils.hpp"
#include <mapnik/image_data.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/image_reader.hpp>
#include <mapnik/map.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/graphics.hpp>
#include <mapnik/save_map.hpp>
#include <mapnik/map.hpp>
#include <mapnik/feature.hpp>
#include <mapnik/feature_factory.hpp>
#include <mapnik/unicode.hpp>
#include <mapnik/geometry.hpp>
#include <mapnik/datasource.hpp>
#include <mapnik/load_map.hpp>


MAPNIK_SHARED_PTR<mapnik::memory_datasource> build_ds(double x,double y, bool second) {
#if MAPNIK_VERSION >= 300000
    mapnik::parameters params;
    params["type"] = "memory";
    MAPNIK_SHARED_PTR<mapnik::memory_datasource> ds = MAPNIK_MAKE_SHARED<mapnik::memory_datasource>(params);
#else
    MAPNIK_SHARED_PTR<mapnik::memory_datasource> ds = MAPNIK_MAKE_SHARED<mapnik::memory_datasource>();
#endif
    mapnik::context_ptr ctx = MAPNIK_MAKE_SHARED<mapnik::context_type>();
    ctx->push("name");
    mapnik::feature_ptr feature(mapnik::feature_factory::create(ctx,1));
    mapnik::transcoder tr("utf-8");
    feature->put("name",tr.transcode("null island"));
    mapnik::geometry_type * pt = new mapnik::geometry_type(MAPNIK_POINT);
    pt->move_to(x,y);
    feature->add_geometry(pt);
    ds->push(feature);
    if (second) {
        ctx->push("name2");
        mapnik::feature_ptr feature(mapnik::feature_factory::create(ctx,1));
        mapnik::transcoder tr("utf-8");
        feature->put("name",tr.transcode("null island"));
        feature->put("name2",tr.transcode("null island 2"));
        mapnik::geometry_type * pt = new mapnik::geometry_type(MAPNIK_POINT);
        pt->move_to(x+1,y+1);
        feature->add_geometry(pt);
        ds->push(feature);
    }
    return ds;
}

namespace testing {

unsigned compare_images(MAPNIK_IMAGE_RGBA const& src1,
                        MAPNIK_IMAGE_RGBA const& src2,
                        int threshold,
                        bool alpha)
{
    unsigned difference = 0;
    unsigned int width = src1.width();
    unsigned int height = src1.height();
    if ((width != src2.width()) || height != src2.height()) return false;
    for (unsigned int y = 0; y < height; ++y)
    {
        const unsigned int* row_from = src1.getRow(y);
        const unsigned int* row_from2 = src2.getRow(y);
        for (unsigned int x = 0; x < width; ++x)
        {
            unsigned rgba = row_from[x];
            unsigned rgba2 = row_from2[x];
            unsigned r = rgba & 0xff;
            unsigned g = (rgba >> 8 ) & 0xff;
            unsigned b = (rgba >> 16) & 0xff;
            unsigned r2 = rgba2 & 0xff;
            unsigned g2 = (rgba2 >> 8 ) & 0xff;
            unsigned b2 = (rgba2 >> 16) & 0xff;
            if (std::abs(static_cast<int>(r - r2)) > threshold ||
                std::abs(static_cast<int>(g - g2)) > threshold ||
                std::abs(static_cast<int>(b - b2)) > threshold) {
                ++difference;
                continue;
            }
            if (alpha) {
                unsigned a = (rgba >> 24) & 0xff;
                unsigned a2 = (rgba2 >> 24) & 0xff;
                if (std::abs(static_cast<int>(a - a2)) > threshold) {
                    ++difference;
                    continue;
                }
            }
        }
    }
    return difference;
}

unsigned compare_images(std::string const& src_fn,
                        std::string const& dest_fn,
                        int threshold,
                        bool alpha)
{
    boost::optional<std::string> type = mapnik::type_from_filename(dest_fn);
    if (!type)
    {
        throw mapnik::image_reader_exception("Failed to detect type of: " + dest_fn);
    }
    MAPNIK_UNIQUE_PTR<mapnik::image_reader> reader1(mapnik::get_image_reader(dest_fn,*type));
    if (!reader1.get())
    {
        throw mapnik::image_reader_exception("Failed to load: " + dest_fn);
    }
    MAPNIK_SHARED_PTR<mapnik::image_32> image_ptr1 = MAPNIK_MAKE_SHARED<mapnik::image_32>(reader1->width(),reader1->height());
    reader1->read(0,0,image_ptr1->data());

    boost::optional<std::string> type2 = mapnik::type_from_filename(src_fn);
    if (!type2)
    {
        throw mapnik::image_reader_exception("Failed to detect type of: " + src_fn);
    }
    MAPNIK_UNIQUE_PTR<mapnik::image_reader> reader2(mapnik::get_image_reader(src_fn,*type2));
    if (!reader2.get())
    {
        throw mapnik::image_reader_exception("Failed to load: " + src_fn);
    }
    MAPNIK_SHARED_PTR<mapnik::image_32> image_ptr2 = MAPNIK_MAKE_SHARED<mapnik::image_32>(reader2->width(),reader2->height());
    reader2->read(0,0,image_ptr2->data());

    MAPNIK_IMAGE_RGBA const& src1 = image_ptr1->data();
    MAPNIK_IMAGE_RGBA const& src2 = image_ptr2->data();
    return compare_images(src1,src2,threshold,alpha);
}

unsigned compare_images(MAPNIK_IMAGE_RGBA const& src1,
                        std::string const& filepath,
                        int threshold,
                        bool alpha)
{
    boost::optional<std::string> type = mapnik::type_from_filename(filepath);
    if (!type)
    {
        throw mapnik::image_reader_exception("Failed to detect type of: " + filepath);
    }
    MAPNIK_UNIQUE_PTR<mapnik::image_reader> reader2(mapnik::get_image_reader(filepath,*type));
    if (!reader2.get())
    {
        throw mapnik::image_reader_exception("Failed to load: " + filepath);
    }
    MAPNIK_SHARED_PTR<mapnik::image_32> image_ptr2 = MAPNIK_MAKE_SHARED<mapnik::image_32>(reader2->width(),reader2->height());
    reader2->read(0,0,image_ptr2->data());

    MAPNIK_IMAGE_RGBA const& src2 = image_ptr2->data();
    return compare_images(src1,src2,threshold,alpha);
}

} // end ns