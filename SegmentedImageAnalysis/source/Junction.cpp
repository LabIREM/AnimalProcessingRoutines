
#include "Junction.h"

int Junction::WIDTH = 1;
int Junction::HEIGHT = 1;

Junction::Junction()
{}

Junction::Junction(unsigned long & _p) : m_Label(_p)
{}

Junction::~Junction()
{}

unsigned long Junction::GetLabel()
{
    return m_Label;
}

void Junction::SetLabel(unsigned long & _p)
{
    m_Label = _p;
}

bool Junction::operator==(const Junction & _p) const
{
    return this->m_Label == _p.m_Label;
}

unsigned int Junction::GetParts()
{
    return  m_Parts;
}

std::vector<unsigned long> Junction::GetCellList()
{
    return m_CellList;
}

std::vector<unsigned long> Junction::GetVerticesList()
{
    return m_VerticesList;
}


std::vector< itk::Index<2> > Junction::GetPixelsList()
{
    return m_PixelsList;
}

std::vector< itk::Index<2> > Junction::GetSmallDilPixelsList()
{
    return m_SmallDilPixelsList;
}

void Junction::SetCellList(std::vector<unsigned long> & _v)
{
    m_CellList = _v;
}

void Junction::SetVerticesList(std::vector<unsigned long> & _v)
{
    m_VerticesList = _v;
}

void Junction::SetPixelsList(std::vector< itk::Index<2> > & _v)
{
    m_PixelsList = _v;
}

void Junction::AddCell(unsigned long & _v)
{
    std::vector<unsigned long>::iterator it;
    it = std::find(m_CellList.begin(), m_CellList.end(), _v);
    if (it == m_CellList.end())
    {
        m_CellList.push_back(_v);
    }
}

void Junction::AddVertex(unsigned long & _v)
{
    std::vector<unsigned long>::iterator it;
    it = std::find(m_VerticesList.begin(), m_VerticesList.end(), _v);
    if (it == m_VerticesList.end())
    {
        m_VerticesList.push_back(_v);
    }
}

void Junction::AddPixel(itk::Index<2> & _v)
{
    std::vector< itk::Index<2> >::iterator it;
    it = std::find(m_PixelsList.begin(), m_PixelsList.end(), _v);
    if (it == m_PixelsList.end())
    {
        m_PixelsList.push_back(_v);
    }
}

void Junction::AddSmallDilPixel(itk::Index<2> & _v)
{
    std::vector< itk::Index<2> >::iterator it;
    it = std::find(m_SmallDilPixelsList.begin(), m_SmallDilPixelsList.end(), _v);
    if (it == m_SmallDilPixelsList.end())
    {
        m_SmallDilPixelsList.push_back(_v);
    }
}

float Junction::GetChordLength()
{
    return m_ChordLength;
}

double Junction::GetJunctionIntensity(unsigned int _p)
{
    return m_Intensities[_p];
}

double Junction::GetJunctionBackgroundIntensity(unsigned int _p)
{
    return m_BgIntensities[_p];
}

void Junction::ComputeIntensities(itk::Image<double, 2>::Pointer image, itk::Image<double, 2>::Pointer bg)
{
    double intensity(0), bgintensity(0);
    std::vector< itk::Index<2> >::const_iterator t;
    for (t = m_SmallDilPixelsList.begin(); t != m_SmallDilPixelsList.end(); ++t)
    {
        double i = image->GetPixel(*t) - bg->GetPixel(*t);
        if (i < 0)
        {
            i = 0;
        }
        intensity += i;
        bgintensity += bg->GetPixel(*t);
    }
    intensity /= m_SmallDilPixelsList.size();
    bgintensity /= m_SmallDilPixelsList.size();
    m_Intensities.push_back(intensity);
    m_BgIntensities.push_back(bgintensity);
}


void Junction::ComputeChordsLength(std::map<unsigned long, Vertex> & _vList, float _scale)
{
    int nbVertices = m_VerticesList.size();
    float dist = 0.0;
    if (nbVertices > 1)
    {
        unsigned int idx1(0), idx2(nbVertices - 1);
        std::vector< unsigned long> newVerticesList;
        newVerticesList.reserve(nbVertices);
        for (int i = 0; i < nbVertices; i++)
        {
            for (int j = i + 1; j < nbVertices; j++)
            {
                unsigned long v1 = m_VerticesList[i];
                unsigned long v2 = m_VerticesList[j];
                itk::FixedArray < float, 2 > v1Index = _vList.find(v1)->second.GetCoordinate();
                itk::FixedArray < float, 2 > v2Index = _vList.find(v2)->second.GetCoordinate();
                float tmp = std::sqrt(std::pow(v1Index[0] - v2Index[0], 2) + std::pow(v1Index[1] - v2Index[1], 2));
                if (tmp >= dist)
                {
                    dist = tmp;
                    idx1 = i;
                    idx2 = j;
                }
            }
        }
        newVerticesList.push_back(m_VerticesList[idx1]);
        for (int i = 0; i < nbVertices; i++)
        {
            if (i != idx1 && i != idx2)
            {
                newVerticesList.push_back(m_VerticesList[i]);
            }
        }
        newVerticesList.push_back(m_VerticesList[idx2]);
        m_VerticesList = newVerticesList;
    }
    m_ChordLength = dist * _scale;
}

std::vector<unsigned long> Junction::GetIndicesPixelsList()
{
    std::vector<unsigned long> res;
    std::vector< itk::Index<2> >::const_iterator it;
    for (it = m_PixelsList.begin(); it != m_PixelsList.end(); ++it)
    {
        res.push_back(sub2ind(WIDTH, HEIGHT, *it));
    }
    return res;
}

std::vector<unsigned long> Junction::GetDilIndicesPixelsList()
{
    std::vector<unsigned long> res;
    std::vector< itk::Index<2> >::const_iterator it;
    for (it = m_SmallDilPixelsList.begin(); it != m_SmallDilPixelsList.end(); ++it)
    {
        res.push_back(sub2ind(WIDTH, HEIGHT, *it));
    }
    return res;
}


unsigned long Junction::GetNeighbor(unsigned int _i)
{
    unsigned long res = 0;
    if (_i == 0)
        res = m_CellList.front();
    else if (_i == 1)
        res = m_CellList.back();
    return res;
}

unsigned long Junction::GetVertex(unsigned int _i)
{
    unsigned long res = 0;
    if (_i == 0)
        res = m_VerticesList.front();
    else if(_i == 1)
        res = m_VerticesList.back();
    return res;
}

void Junction::SortCellList()
{
    std::sort(m_CellList.begin(), m_CellList.end());
}
