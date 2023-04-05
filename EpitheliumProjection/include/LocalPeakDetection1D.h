#ifndef __localpeakdetection1D_h
#define __localpeakdetection1D_h

template< class T1, class T2 >
class LocalPeakDetection1D
{

public:
    
    LocalPeakDetection1D(std::vector< T1 > & _v, std::vector< T2 > & _i);
    LocalPeakDetection1D(std::vector< T1 > & _v, std::vector< T2 > & _i, double _d);

    T2 GetFirstPeak();
    T2 GetSecondPeak();
    T2 GetMaxPeak();

private:
    void detect_peak(bool emi_first);

    std::vector< T2 > indexList;
    std::vector< T1 > valueList;
    std::vector< int > emiPeaks;
    std::vector< int > absPeaks;
    double delta;
};


template< class T1, class T2 >
LocalPeakDetection1D<T1, T2>::LocalPeakDetection1D(std::vector< T1 > & _v, std::vector< T2 > & _i)
{
    valueList = _v;
    indexList = _i;
    delta = 0;
}

template< class T1, class T2 >
LocalPeakDetection1D<T1, T2>::LocalPeakDetection1D(std::vector< T1 > & _v, std::vector< T2 > & _i, double _d)
{
    valueList = _v;
    indexList = _i;
    delta = _d;
}

template< class T1, class T2 >
T2
LocalPeakDetection1D<T1, T2>::GetMaxPeak()
{
    typename std::vector<T1>::iterator res = std::max_element(valueList.begin(), valueList.end());
    int resIdx = res - valueList.begin();
    return indexList[resIdx];
}

template< class T1, class T2 >
T2
LocalPeakDetection1D<T1, T2>::GetFirstPeak()
{
    detect_peak(true);
    T2 result = 0;
    if (!emiPeaks.empty())
    {
        result = indexList[emiPeaks[0]];
    }
    else
    {
        result = GetMaxPeak();
    }
    return result;
}


template< class T1, class T2 >
T2
LocalPeakDetection1D<T1, T2>::GetSecondPeak()
{
    detect_peak(true);
    T2 result = 0;
    if (!emiPeaks.empty())
    {
        if (emiPeaks.size() >= 2)
        {
            result = indexList[emiPeaks[1]];
        }
        else
        {
            result = indexList[emiPeaks[0]];
        }
    }
    else
    {
        result = GetMaxPeak();
    }
    return result;
}


template< class T1, class T2 >
void 
LocalPeakDetection1D<T1, T2>::detect_peak(bool emi_first)
{
    int mx_pos = 0;
    int mn_pos = 0;
    double mx = valueList[0];
    double mn = valueList[0];

    //if (delta == -1)
    //{
    //    delta = accumulate(valueList.begin(), valueList.end(), 0.0) / valueList.size();
    //}
    
    for (int i = 1; i < valueList.size(); i++)
    {
        if (valueList[i] > mx) 
        {
            mx_pos = i;
            mx = valueList[i];
        }
        if (valueList[i] < mn) 
        {
            mn_pos = i;
            mn = valueList[i];
        }

        if (emi_first && valueList[i] < mx - delta)
        {
            emiPeaks.push_back(mx_pos);

            emi_first = false;

            i = mx_pos - 1;

            mn = valueList[mx_pos];
            mn_pos = mx_pos;
        }
        else if ((!emi_first) && valueList[i] > mn + delta)
        {
            absPeaks.push_back(mn_pos);

            emi_first = true;

            i = mn_pos - 1;

            mx = valueList[mn_pos];
            mx_pos = mn_pos;
        }
    }
}





#endif // __localpeakdetection1D_h
