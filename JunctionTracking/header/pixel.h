/*** Class Pixel.
 *
 * Represents a pixel of the segmentation skeleton.
 * A pixel will be define by an index, a list of neighbour cell, and a vertex indication
 *
 * @author Stephane Rigaud
 * @version 2016-05-27
 */

#ifndef __pixel_h
#define __pixel_h

#include "tools.h"
#include <string>
#include <vector>
#include <utility>

class Pixel
{
    unsigned int m_Id;                  // pixel id
    unsigned int m_NeighboursCellId[4]; // neighbouring cells id number (max 4)
    bool         m_Vertex;              // define pixel as vertex or simple pixel

    static unsigned int WIDTH;          // image width, static and define at execution
    static unsigned int HEIGHT;         // image height, static and define at execution

public:

    /* Constructor and Destructor */
    Pixel();
    Pixel(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
    Pixel(const Pixel &);
    ~Pixel(){};

    /* Operator */
    bool operator== (const Pixel &) const;
    bool operator== (const unsigned int) const;
    bool operator>  (const Pixel &) const;
    bool operator<  (const Pixel &) const;

    /* Get */
    unsigned int GetId() const;
    static unsigned int GetWidth();
    static unsigned int GetHeight();
    unsigned int GetNeighbour(unsigned int) const;
    unsigned int GetNbNeighbours() const;
    std::pair<unsigned int, unsigned int> GetCoordinate() const;
    std::vector< std::pair<unsigned int, unsigned int> > GetNeighboursCoupleList() const;
    
    /* Set */
    void Set(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
    static void SetWidth(unsigned int); 
    static void SetHeight(unsigned int); 

    /* Misc */
    void Clear();
    bool IsVertex() const;
    bool Is8Connex(const unsigned int) const;
    bool Is4Connex(const unsigned int) const;
    bool Is8Connex(const Pixel &) const;
    bool Is4Connex(const Pixel &) const;
    void FillNeighboursCoupleList(std::vector<std::pair<unsigned int, unsigned int> > &) const;
    std::string ToString() const;
    void FillString(std::string &s) const;

};

#endif // __pixel_h
