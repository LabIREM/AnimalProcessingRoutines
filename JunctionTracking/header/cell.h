/*** Class Cell
 *
 * Represents a cell
 *
 * @author Stephane Rigaud
 * @version 2016-06-06
 */

#ifndef __cell_h
#define __cell_h

#include <string>

#include "border.h"

class Cell
{
    int m_Category;              // cell category
    std::string m_AbsoluteTag;   // cell Absolute Tag
    unsigned int m_NbNeighbours; // number of neighbours
    
    std::vector<std::string> m_NeighboursList; // Neighbour cell list ?
    std::vector<Border> m_BordersList;         // Border list
    
    double m_TotalIntensity; // total mean borders intensity
    double m_IntegratedIntensity;// integrated intensity (w/ dilated border)
    double m_ErodedArea;// area (w/ dilated border)
    
public:
    
    // BORDERCELL category
    static const int BORDERCELL = 1;
    static const int DEFAULTCATEGORY = -1;
    
    // constructors
    Cell();
    Cell(int);
    
    // getter
    double GetMeanBorderIntensity();
    int GetCategory();
    std::string GetAbsoluteTag();
    double GetErodeArea();
    double GetIntegratedIntensity();
    unsigned int GetNbNeighbours();
    Border GetBorder(unsigned int);
    
    // setter
    void SetCategory(int);
    void SetAbsoluteTag(std::string);
    void SetIntegratedIntensity(double);
    void SetErodeArea(double);

    // other methods
    bool ContainBorder(unsigned int);
    bool ContainBorder(Border &);
    void AddBorder(Border &);
    std::string ToString();
    bool IsEmpty();
    void Clear();
    
};

#endif // __cell_h
