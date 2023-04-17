/**
* \class Junction
*
*
* \brief Class regrouping all information related to a junction in a tissu
*
* This class gathered all information related to a junction, from its position
* in the image, its length, or pixels, but also its relationship with
* other component of a tissu, such as other cells, and vertices.
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

#ifndef __JUNCTION_H
#define __JUNCTION_H

#include <vector>
#include "itkImage.h"

#include "Vertex.h"
#include "Utility.h"

class Junction
{

public:

    static int WIDTH;
    static int HEIGHT;

    typedef unsigned long    LabelType;
    typedef itk::Index < 2 > IndexType;
    typedef itk::FixedArray < float, 2 > PointType;

    Junction();
    Junction(LabelType &);
    ~Junction();

    LabelType GetLabel();
    void SetLabel(LabelType &);

    bool operator == (const Junction &) const;

    unsigned int GetParts();
    float GetChordLength();
    double GetJunctionIntensity(unsigned int);
    double GetJunctionBackgroundIntensity(unsigned int);
    std::vector < LabelType > GetCellList();
    std::vector < LabelType > GetVerticesList();
    std::vector < LabelType > GetIndicesPixelsList();
    std::vector < LabelType > GetDilIndicesPixelsList();
    std::vector < IndexType > GetPixelsList();
    std::vector < IndexType > GetSmallDilPixelsList();

    void SetCellList(std::vector < LabelType > &);
    void SetVerticesList(std::vector < LabelType > &);
    void SetPixelsList(std::vector < IndexType > &);

    LabelType GetNeighbor(unsigned int);
    LabelType GetVertex(unsigned int);

    void AddCell(LabelType &);
    void AddVertex(LabelType &);
    void AddPixel(IndexType &);
    void AddSmallDilPixel(IndexType &);

    void ComputeIntensities(itk::Image < double, 2 > ::Pointer, itk::Image < double, 2 > ::Pointer);
    void ComputeChordsLength(std::map < LabelType, Vertex > &, float);

    void SortCellList();

private:

    LabelType m_Label;
    unsigned int m_Parts = 1;
    float m_ChordLength = 0;

    std::vector< double > m_Intensities;
    std::vector< double > m_BgIntensities;

    std::vector < LabelType > m_CellList;
    std::vector < LabelType > m_VerticesList;
    std::vector < IndexType > m_PixelsList;
    std::vector < IndexType > m_SmallDilPixelsList;

}; // end of class Junction

#endif // __JUNCTION_H
