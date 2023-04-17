/**
* \class Vertex
*
*
* \brief Class regrouping all information related to a vertex in a tissu
*
* This class gathered all information related to a vertex, its position
* in the image, but also its relationship with other component of a tissu,
* such as junctions and cells.
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

#ifndef __VERTEX_H
#define __VERTEX_H

#include "itkImage.h"
#include "Utility.h"

class Vertex 
{

public:

    typedef unsigned long    LabelType;
    typedef itk::Index < 2 > IndexType;
    typedef itk::FixedArray < float, 2 > PointType;

    Vertex();
    Vertex(LabelType &);
    ~Vertex();

    LabelType GetLabel();
    void SetLabel(LabelType &);

    bool isNeighbor(LabelType &);
    void AddCell(LabelType &);
    bool operator == (const Vertex &) const;

    std::vector < LabelType > GetCellList();
    PointType GetCoordinate();
    LabelType GetLinearIndex();

    void SetCoordinate(PointType &);
    void SetCoordinate(float, float);
    void SetLinearIndex(LabelType &);
    void SetCellList(std::vector < LabelType > &);

private:

    LabelType m_Label;
    LabelType m_LinearIndex;
    PointType m_Coordinate;
    std::vector < LabelType > m_CellList;

}; // end of class Vertex

#endif // __VERTEX_H