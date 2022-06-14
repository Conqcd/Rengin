#include "repch.hpp"
#include "BVHMortonCompute.hpp"
#include <float.h>
#include <cstdio>


namespace Rengin
{
unsigned int __clz_32(unsigned int v)
{
    if(!v)  return 32;
    int e = 31;
    if(v&0xffff0000)    {   e-= 16; v >> 16;}
    if(v&0x0000ff00)    {   e-= 8; v >> 8;}
    if(v&0x000000f0)    {   e-= 4; v >> 4;}
    if(v&0x0000000C)    {   e-= 2; v >> 2;}
    if(v&0x00000002)    {   e-= 1;}
    return e;
}

unsigned int __clz_64(unsigned long long v)
{
	unsigned v1, v2;
	v1 = v;
	v2 = v >> 32;
	if(v2 != 0)
	{
		return __clz_32(v2);
	}else
	{
		return 32 + __clz_32(v1);
	}
}

unsigned long long expandBits_long(unsigned long long v)
{
	v = (v * 0x0000000100000001llu) & 0xFFFF00000000FFFFllu;
	v = (v * 0x00010001llu) & 0x00FF0000FF0000FFllu;
	v = (v * 0x00000101llu) & 0xF00F00F00F00F00Fllu;
	v = (v * 0x00000011llu) & 0x30C30C30C30C30C3llu;
	v = (v * 0x00000005llu) & 0x9249249249249249llu;
	return v;
}

unsigned long long morton3d_double(double x,double y,double z)
{
    x = fmin(fmax(x * 2097152.f, 0.0f), 2097151.f);
	y = fmin(fmax(y * 2097152.f, 0.0f), 2097151.f);
	z = fmin(fmax(z * 2097152.f, 0.0f), 2097151.f);
	unsigned long long xx = expandBits_long((unsigned long long)x);
	unsigned long long yy = expandBits_long((unsigned long long)y);
	unsigned long long zz = expandBits_long((unsigned long long)z);
	return xx * 4 + yy * 2 + zz;
}

long long clz_index_long(long long idx, long long idy, unsigned int NumObjects,const std::vector<std::pair<unsigned long long,int>>& sortedMortonCodes)
{
	return sortedMortonCodes[idx] == sortedMortonCodes[idy]
		? (NumObjects - std::max(idx, idy)) + 64
		: __clz_64(sortedMortonCodes[idx].first ^ sortedMortonCodes[idy].first);
}

long long clz_safe_long(long long idx, long long idy, unsigned int NumObjects,const std::vector<std::pair<unsigned long long,int>>& sortedMortonCodes)
{
	if (idy < 0 || idy > NumObjects - 1) return -1;
	return clz_index_long(idx, idy, NumObjects, sortedMortonCodes);
}

std::pair<long long,long long> determineRange_long(const std::vector<std::pair<unsigned long long,int>>& sortedMortonCodes, size_t numObjects, unsigned int i)
{
	long long d = (clz_safe_long(i, i + 1, numObjects, sortedMortonCodes) - clz_safe_long(i, i - 1, numObjects, sortedMortonCodes)) > 0 ? 1 : -1;
	long long commonPrefixMin = clz_safe_long(i, i - d, numObjects, sortedMortonCodes);
	long long l_max = 2;
	while (clz_safe_long(i, i + d * l_max, numObjects, sortedMortonCodes) > commonPrefixMin)
	{
		l_max *= 2;
	}
	long long l = 0;
	long long t = l_max;
	do
	{
		t = (t + 1) >> 1;
		if (clz_safe_long(i, i + d * (l + t), numObjects, sortedMortonCodes) > commonPrefixMin)
		{
			l += t;
		}
	} while (t > 1);
	long long j = i + l * d;
    if(d > 0)
        return {i, j};
    else
        return {j,i};
}
long long findSplit_long(const std::vector<std::pair<unsigned long long,int>>& sortedMortonCodes,long long first,long long last) {
	unsigned long long firstCode = sortedMortonCodes[first].first;
	unsigned long long lastCode = sortedMortonCodes[last].first;

	if (firstCode == lastCode)
		return (first + last) >> 1;

	long long commonPrefix = __clz_64(firstCode ^ lastCode);

	long long split = first;
	long long step = last - first;

	do
	{
		step = (step + 1) >> 1;
		long long newSplit = split + step;

		if (newSplit < last)
		{
			unsigned long long splitCode = sortedMortonCodes[newSplit].first;
			long long splitPrefix = __clz_64(firstCode ^ splitCode);
			if (splitPrefix > commonPrefix)
				split = newSplit;
		}
	} while (step > 1);
	return split;
}
void ComputeBVHandMorton(std::vector<int> &indices,std::vector<int> &newindices,const std::vector<float> &vertices,std::vector<float> &leafNodes,std::vector<Node>& internalNodes)
{
    glm::vec3 box_min(FLT_MAX,FLT_MAX,FLT_MAX),box_max(FLT_MIN,FLT_MIN,FLT_MIN);
    int size = indices.size() / 3;
    leafNodes.reserve(size * 6);
    std::vector<std::pair<unsigned long long,int>> mortonCode;
    mortonCode.reserve((size));
    newindices.reserve(size);
    internalNodes.resize(size - 1);
    for (int i = 0; i < size; i++)
    {
        int id1 = indices[i * 3],id2 = indices[i * 3 + 1],id3 = indices[i * 3 + 2];
        glm::vec3 aabbmin(vertices[id1],vertices[id1 + 1],vertices[id1 + 2]),aabbmax(vertices[id1],vertices[id1 + 1],vertices[id1 + 2]);

        aabbmin[0] = std::min(aabbmin[0],vertices[id2]);
        aabbmax[0] = std::max(aabbmax[0],vertices[id2]);
        aabbmin[1] = std::min(aabbmin[1],vertices[id2 + 1]);
        aabbmax[1] = std::max(aabbmax[1],vertices[id2 + 1]);
        aabbmin[2] = std::min(aabbmin[2],vertices[id2 + 2]);
        aabbmax[2] = std::max(aabbmax[2],vertices[id2 + 2]);

        aabbmin[0] = std::min(aabbmin[0],vertices[id3]);
        aabbmax[0] = std::max(aabbmax[0],vertices[id3]);
        aabbmin[1] = std::min(aabbmin[1],vertices[id3 + 1]);
        aabbmax[1] = std::max(aabbmax[1],vertices[id3 + 1]);
        aabbmin[2] = std::min(aabbmin[2],vertices[id3 + 2]);
        aabbmax[2] = std::max(aabbmax[2],vertices[id3 + 2]);

        box_min[0] = std::min(box_min[0],aabbmin[0]);
        box_max[0] = std::max(box_max[0],aabbmax[0]);
        box_min[1] = std::min(box_min[1],aabbmin[1]);
        box_max[1] = std::max(box_max[1],aabbmax[1]);
        box_min[2] = std::min(box_min[2],aabbmin[2]);
        box_max[2] = std::max(box_max[2],aabbmax[2]);

        leafNodes.push_back(aabbmin[0]);
        leafNodes.push_back(aabbmin[1]);
        leafNodes.push_back(aabbmin[2]);
        leafNodes.push_back(aabbmax[0]);
        leafNodes.push_back(aabbmax[1]);
        leafNodes.push_back(aabbmax[2]);
    }
    for (int i = 0; i < size; i++)
    {
        int id1 = indices[i * 3],id2 = indices[i * 3 + 1],id3 = indices[i * 3 + 2];
        double xx = 0,yy = 0,zz = 0;

        xx += vertices[id1];
        xx += vertices[id2];
        xx += vertices[id3];

        yy += vertices[id1 + 1];
        yy += vertices[id2 + 1];
        yy += vertices[id3 + 1];

        zz += vertices[id1 + 2];
        zz += vertices[id2 + 2];
        zz += vertices[id3 + 2];

        xx /= 3;
        yy /= 3;
        zz /= 3;

        mortonCode.emplace_back(morton3d_double((xx - box_min[0]) / (box_max[0] - box_min[0]),(yy - box_min[1]) / (box_max[1] - box_min[1]),(zz - box_min[2]) / (box_max[2] - box_min[2])),i);
    }
    std::sort(mortonCode.begin(),mortonCode.end());
    for (int i = 0; i < size; i++)
    {
        newindices.push_back(mortonCode[i].second);
    }
    
    
    for (int i = 0; i < size - 1; i++)
    {
        std::pair<long long,long long> range = determineRange_long(mortonCode, size, i);
        long long first = range.first;
        long long last = range.second;


        long long split = findSplit_long(mortonCode, first, last);

        if (split == first)
            internalNodes[i].lleaf = newindices[split],
            internalNodes[i].lchild = -1;
        else
            internalNodes[i].lleaf = -1,
            internalNodes[i].lchild = split;

        if (split + 1 == last)
            internalNodes[i].rleaf = newindices[split + 1],
            internalNodes[i].rchild = -1;
        else
            internalNodes[i].rleaf = -1,
            internalNodes[i].rchild = split + 1;
        for (long long j = first; j <= last; j++)
        {
            int leafid = newindices[j];
            if (j == first)
            {
                internalNodes[i].bmin[0] = leafNodes[leafid * 6];
                internalNodes[i].bmin[1] = leafNodes[leafid * 6 + 1];
                internalNodes[i].bmin[2] = leafNodes[leafid * 6 + 2];
                internalNodes[i].bmax[0] = leafNodes[leafid * 6 + 3];
                internalNodes[i].bmax[1] = leafNodes[leafid * 6 + 4];
                internalNodes[i].bmax[2] = leafNodes[leafid * 6 + 5];
            }
            else
            {
                internalNodes[i].bmin[0] = std::min(leafNodes[leafid * 6],internalNodes[i].bmin[0]);
                internalNodes[i].bmin[1] = std::min(leafNodes[leafid * 6 + 1],internalNodes[i].bmin[1]);
                internalNodes[i].bmin[2] = std::min(leafNodes[leafid * 6 + 2],internalNodes[i].bmin[2]);
                internalNodes[i].bmax[0] = std::max(leafNodes[leafid * 6 + 3],internalNodes[i].bmax[0]);
                internalNodes[i].bmax[1] = std::max(leafNodes[leafid * 6 + 4],internalNodes[i].bmax[1]);
                internalNodes[i].bmax[2] = std::max(leafNodes[leafid * 6 + 5],internalNodes[i].bmax[2]);
            }
        }
    }
}

} // namespace Rengin
