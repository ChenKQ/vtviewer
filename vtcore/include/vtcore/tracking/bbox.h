#ifndef BBOX_H
#define BBOX_H

#include <string>

namespace vtcore
{
namespace tracking
{

typedef struct BBox
{
    std::string name;
    float prob;
    int minx;
    int maxx;
    int miny;
    int maxy;
} BBox;

}   // namespace tracking
}   // namespace vtcore

#endif // BBOX_H
