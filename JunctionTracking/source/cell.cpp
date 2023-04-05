/*** Class Cell.
 *
 * Represents a cell.
 *
 * @author Stephane Rigaud
 * @version 2016-06-06
 */

#include <sstream>

#include "cell.h"


Cell::Cell()
{
    m_TotalIntensity = 0;
    m_Category = DEFAULTCATEGORY;
    m_AbsoluteTag = "";
    m_ErodedArea = 0;
    m_IntegratedIntensity = 0;
    m_NbNeighbours = 0;
}

Cell::Cell(int ctg)
{
    m_TotalIntensity = 0;
    m_Category = ctg;
    m_AbsoluteTag = "";
    m_ErodedArea = 0;
    m_IntegratedIntensity = 0;
    m_NbNeighbours = 0;
}

void Cell::AddBorder(Border & _b)
{
    m_TotalIntensity += _b.GetMeanIntensity();
    m_BordersList.push_back(_b);
    m_NbNeighbours = m_BordersList.size();
}

Border Cell::GetBorder( unsigned int _id)
{
    Border theBorder = Border();
    std::vector<Border>::iterator ite = find(m_BordersList.begin(),m_BordersList.end(), _id);
    if (ite != m_BordersList.end())
    {
        theBorder = (*ite);
    }
    return theBorder;
    
}

double Cell::GetMeanBorderIntensity()
{
    return m_TotalIntensity / double(m_NbNeighbours);
}

void Cell::SetCategory(int _ctgr)
{
    m_Category = _ctgr;
}

int Cell::GetCategory()
{
    return m_Category;
}

std::string Cell::ToString()
{
    std::ostringstream out;
    out << GetAbsoluteTag()         << ","
        << GetMeanBorderIntensity() << ","
        << m_IntegratedIntensity    << ","
        << m_ErodedArea             << ","
        << m_NbNeighbours;
    return out.str();
}

bool Cell::IsEmpty()
{
    return (m_NbNeighbours == 0);
}

std::string Cell::GetAbsoluteTag()
{
    return m_AbsoluteTag;
}

void Cell::SetAbsoluteTag(std::string _tag)
{
    m_AbsoluteTag = _tag;
}

void Cell::SetErodeArea(double _a)
{
    m_ErodedArea = _a;
}

double Cell::GetErodeArea()
{
    return m_ErodedArea;
}

void Cell::SetIntegratedIntensity(double _i)
{
    m_IntegratedIntensity = _i;
}

double Cell::GetIntegratedIntensity()
{
    return m_IntegratedIntensity;
}

void Cell::Clear()
{
    m_TotalIntensity = 0;
    m_Category = DEFAULTCATEGORY;
    m_AbsoluteTag = "";
    m_IntegratedIntensity = 0;
    m_ErodedArea = 0;
    m_NbNeighbours = 0;
    m_BordersList.clear();
}

bool Cell::ContainBorder(unsigned int _id)
{
    bool flag = false;
    std::vector<Border>::iterator ite = find(m_BordersList.begin(),m_BordersList.end(), _id);
    if (ite != m_BordersList.end())
    {
        flag = true;
    }
    return flag;
}

bool Cell::ContainBorder(Border & _b)
{
    bool flag = false;
    std::vector<Border>::iterator ite = find(m_BordersList.begin(),m_BordersList.end(), _b);
    if (ite != m_BordersList.end())
    {
        flag = true;
    }
    return flag;
}

//end of file