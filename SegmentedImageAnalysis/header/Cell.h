/**
* \class Cell
*
*
* \brief Class regrouping all information related to a cell in a tissu
*
* This class gathered all information related to a cell, from its position
* in the image, its area, or perimeter, but also its relationship with
* other component of a tissu, such as other cells (neighbors), junctions,
* and vertices.
*
* \author $Author: Stephane Rigaud $
*
* \version $Revision: 1.0 $
*
* \date $Date: 2018/04/04 $
*
* Contact: stephane.rigaud@curie.fr
*
*/


#ifndef __CELL_H
#define __CELL_H

#include <numeric>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>

#define _USE_MATH_DEFINES
#include <math.h>

#include "itkImage.h"
#include "Junction.h"
#include "Vertex.h"

#include "Utility.h"


class Cell 
{

public:

    static int WIDTH;
    static int HEIGHT;

    enum CATEGORY 
    {
        CORECELL = 0,
        FIRSTLAYER = 1,
        BORDERCELL = 2,
    };

    typedef unsigned long    LabelType;
    typedef itk::Index < 2 > IndexType;
    typedef itk::FixedArray < float, 2 > PointType;


    Cell();
    Cell(LabelType &);
    Cell(LabelType &, CATEGORY);
    ~Cell();

    LabelType GetLabel();
    void SetLabel(LabelType &);

    int  GetCategory();
    void SetCategory(int);
    void SetCategory(CATEGORY);

    bool operator == (const Cell &) const;

    void AddNeighbor(LabelType &);
    void AddJunction(LabelType &);
    void AddVertex  (LabelType &);
    void AddPixel   (IndexType &);

    std::vector < LabelType > GetNeighborsList();
    std::vector < LabelType > GetJunctionsList();
    std::vector < LabelType > GetVerticesList();
    std::vector < IndexType > GetPixelsList();
    std::vector < IndexType > GetSmallDilPixelsList();
    std::vector < LabelType > GetIndicesPixelsList();
    std::vector < LabelType > GetJunctionsIndicesPixelsList();
    std::vector < LabelType > GetDilJunctionsIndicesPixelsList();

    void SetNeighborsList(std::vector < LabelType > &);
    void SetJunctionsList(std::vector < LabelType > &);
    void SetVerticesList (std::vector < LabelType > &);
    void SetPixelsList   (std::vector < IndexType > &);

    PointType GetCentroid();
    float GetArea();
    float GetPerimeter();
    float GetElongation();
    float GetEccentricity();
    float GetRoundness();
    float GetAnisotropy();
    float GetOrientation();
    float GetMajorAxisLength();
    float GetMinorAxisLength();
    float GetChordLength();
    float GetChordDisorder();
    double GetMaximumIntensity();
    double GetMinimumIntensity();
    double GetAverageIntensity();
    double GetVarianceIntensity();
    double GetContourIntensity();
    double GetBackgroundIntensity();
    float GetInertiaAnisotropy();
    float GetInertiaOrientation();
    float GetVertexOrientation();
    float GetVertexPolarity();
    double GetContourIntensity(unsigned int);
    double GetBackgroundIntensity(unsigned int);
    double GetContourIntensityDisorder(unsigned int);

    std::vector<double> GetPolarityMode(unsigned int);
    std::vector<float> GetInertiaTensor();
    std::vector<float> GetTextureTensor();
    std::vector<float> GetVertexTensor();

    void SetArea(float &);
    void SetPerimeter(float &);
    void SetElongation(float &);
    void SetEccentricity(float &);
    void SetRoundness(float &);
    void SetAnisotropy(float &);
    void SetOrientation(float &);
    void SetMajorAxisLength(float &);
    void SetMinorAxisLength(float &);
    void SetMaximumIntensity (double &);
    void SetMinimumIntensity (double &);
    void SetAverageIntensity (double &);
    void SetVarianceIntensity(double &);
    void SetCentroid(float &, float &);

    void ComputeIntensities(itk::Image < double, 2 > ::Pointer, itk::Image < double, 2 > ::Pointer);
    void ComputePolarityMode(itk::Image < double, 2 > ::Pointer, itk::FixedArray < unsigned int, 2 > &, bool);
    void ComputeCategory(std::map < LabelType, Cell > &);
    void ComputeInertia(float, itk::FixedArray < unsigned int, 2 > &);
    void ComputeTexture(std::map < LabelType, Cell > &, std::map < LabelType, Junction > &, float, itk::FixedArray < unsigned int, 2 > &);
    void ComputeVertices(std::map < LabelType, Vertex > &, itk::FixedArray < unsigned int, 2 > &);
    void ComputeChords(std::map < LabelType, Junction > &);
    void ComputeJunctionsPixelsList(std::map < LabelType, Junction > &);

private:

    LabelType m_Label;
    CATEGORY m_Category;

    std::vector < LabelType > m_NeighborsList;
    std::vector < LabelType > m_VerticesList;
    std::vector < LabelType > m_JunctionsList;
    std::vector < IndexType > m_PixelsList;
    std::vector < IndexType > m_SmallDilPixelsList;
    std::vector < IndexType > m_JunctionsPixelsList;

    PointType m_Centroid;
    float m_Area = 0;
    float m_Perimeter = 0;
    float m_Elongation = 0;
    float m_Eccentricity = 0;
    float m_Roundness = 0;
    float m_Anisotropy = 0;
    float m_Orientation = 0;
    float m_MajorAxisLength = 0;
    float m_MinorAxisLength = 0;
    float m_ChordLength = 0;
    float m_ChordDisorder = 0;
    double m_MaximumIntensity = 0;
    double m_MinimumIntensity = 0;
    double m_AverageIntensity = 0;
    double m_VarianceIntensity = 0;
    double m_ContourIntensity = 0;
    double m_BackgroundIntensity = 0;
    float m_InertiaAnisotropy = 0;
    float m_InertiaOrientation = 0;
    float m_VertexPolarity = 0;
    float m_VertexOrientation = 0;

    std::vector< float > m_InertiaTensor;
    std::vector< float > m_TextureTensor;
    std::vector< float > m_VertexTensor;
    std::vector< double > m_BackgroundIntensityList;
    std::vector< double > m_ContoursIntensityList;
    std::vector< double > m_ContoursIntensityDisordersList;

    std::vector< std::vector<double> > m_PolarityModesList;

}; // end of class Cell

#endif //__CELL_H