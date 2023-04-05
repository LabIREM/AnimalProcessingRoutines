/*** Class Border
 *
 * Represents a border between 2 cells
 * A border is composed of a list of pixel
 *
 * @author Stephane Rigaud
 * @version 2016-05-27
 */

#ifndef __border_h
#define __border_h

#include <string>
#include <vector>
#include <utility>
#include <set>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <sstream>
#include <cmath>       
#include <queue>     
#include <list>
#include <sstream>
#include <fstream>

#include <itkImage.h>  
#include <itkImageRegionConstIterator.h>

#include "tools.h"
#include "pixel.h"


class Border
{

private:

    unsigned int m_Id;              // border's id
    unsigned int m_Neighbours[2];   // neighbour id
    std::vector<Pixel> m_PixelList; // pixels index
    std::string m_AbsoluteTag;      // border's absolute tag

    unsigned int m_NbVertex;
    unsigned int m_LocalNbPixels;
    unsigned int m_NbDilatedPixels;
    float m_Length;
    float m_TotalIntensity;
    float m_IntensityRatio;
    float m_LocalBackground;
    float m_LocalTotalIntensity;
    float m_Angle;
    float m_Cord;
    float m_Curvature;

public:

    // typedef
    typedef itk::Image<unsigned int, 2>           ImageType;
    typedef std::pair<unsigned int, unsigned int> CoordinatePixelType;
    typedef std::vector<Pixel>                    PixelListType;
    typedef std::vector<Pixel>::iterator          PixelListIteratorType;
    typedef std::vector<Pixel>::const_iterator    PixelListConstIteratorType;
    typedef std::vector<Pixel>::reverse_iterator  PixelListReverseIteratorType;
    
    // constructor
    Border();
    
    
    // operator
    bool operator== (const Border &) const;
    bool operator== (const unsigned int) const;


    // Getters
    unsigned int GetId() const;
    unsigned int GetFirstNeighbour() const;
    unsigned int GetSecondNeighbour() const;
    unsigned int * GetNeighbours();
    unsigned int GetNbVertex();
    PixelListType GetPixelList() const;
    std::string GetAbsoluteTag() const;
    double GetMeanIntensity() const;
    

    // Setters
    void Set(const unsigned int, const unsigned int, const unsigned int);
    void AddPixel(const Pixel &);
    void SetAbsoluteTag(const std::string);
    void SetPixels(const PixelListType &);
    void SetNbVertex();
    void SetLength();
    void SetAngle();
    void SetCord();
    void SetCurvature();
    void SetTotalIntensity(const float);
    void SetNbDilatedPixels(const float);
    void SetIntensityRatio(const float, const float);
    void SetLocalBackground(const float);
    void SetLocalNbPixels(const float);
    void SetLocalTotalIntensity(const float);

    
    // Misc
    std::string ToString();
    std::pair<double, double> GetCentroid() const;
    void GetCentroid(std::pair<double, double> &) const;
    bool IsLinked(unsigned int) const;
    bool IntegrityTest() const;
    PixelListType Difference(const PixelListType &) const;
    std::string PixelListToString(const bool) const;
    PixelListType SortPixel();
    std::pair<float, float> MeanIntensity(const ImageType *, const unsigned int) const;
};

#endif // __border_h

//unsigned int nbVertex;              // number of vertex
//double length;                      // length
//unsigned int totalIntensity;        // total intensity (dilated border)
//unsigned int nbDilatedPixels;       // number of pixels of dilated border
//double intensityRatio;              // border/cell intensity ratio
//double localBackground;             // local mean background intensity
//unsigned int localTotalIntensity;   // local total intensity
//unsigned int localNbPixels;         // number of pixels for local total intensity

// private methods
// std::string PixelListToString(bool);

//unsigned int GetNbVertex();    
//double GetMeanIntensity();
//void SetLength(bool);
//void SetIntensityValues(unsigned int, unsigned int);
//void SetIntensityRatio(double);
//void SetLocalBackground(double);
//void SetLocalIntensity(unsigned int, unsigned int);
//void SetNbVertex(unsigned int);
