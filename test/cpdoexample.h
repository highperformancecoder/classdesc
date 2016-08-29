struct CPdo {};

typedef unsigned UINT;
typedef unsigned SYSTEMTIME;
typedef unsigned long ULONG;

#include "classdesc_access.h"
#include <string>
#include <vector>

class CRegionDetailsPdo : public CPdo
{
CLASSDESC_ACCESS(CRegionDetailsPdo);
public:
static const std::string REGION_DETAILS_COLLECTION;

/** Structure TcsDataType records the relationship between a site and subsystem. */
typedef struct TcsDataType
{
    UINT theTcs;
    UINT theSubsystem;

    TcsDataType ()
    {
        theTcs = 0;
        theSubsystem = 0;
    } // TcsDataType constructor

}  TcsData;

// member variables
protected:
  /* m_theRegionNumber is the SCATS allocated region number and can change. It cannot be used
* as a direct identifier for this Region. */
UINT m_theRegionNumber;
UINT m_theGuiPort;
UINT m_theMinorRevision;
UINT m_theMajorRevision;
UINT m_theVersion;
UINT m_theSiteCount;
std::string m_theRegionName;
std::string m_theIpAddress;
  /* m_theRegionLicenseOption is one of FixedTimeSupport, AdaptiveSupport, CMSSupport, DIDOSupport, ANTTSSupport */
std::string m_theRegionLicenseOption;
/* m_theTcsList is the list of sites associated with this region. */
std::vector<TcsData> m_theTcsList;
  /* m_theUpdateTime is the time the region details was last updated. */
SYSTEMTIME m_theUpdateTime;

// Methods

public:
/*
* Constructor CRegionDetailsPdo ensures the member variables are initialised.

*/
CRegionDetailsPdo ();

CRegionDetailsPdo (std::string theId);

CRegionDetailsPdo (std::string theId, ULONG theVersion);

/**
   * Method ~CRegionDetailsPdo is the destructor for the instance. 
   */
~CRegionDetailsPdo ();

    void setRegionName (std::string theRegionName);
    std::string getRegionName () const;

    void setRegionNumber (UINT theRegionNumber);
    UINT getRegionNumber () const;

    void setIpAddress (std::string theIpAddress, UINT thePort);
    void getIpAddress (std::string& theIpAddress, UINT& thePort) const;

    /**
     * Method getSCATTermPort returns the port for the SCAT Term connection
   * to  this Region.
     * thePort returns the port number to use to connect via the SCATTerm protocol
     * to this Region.
     */
    void getSCATTermPort (UINT& thePort) const;

    void setSiteCount (UINT theSiteCount);
    UINT getSiteCount () const;

    void setRegionVersion (UINT theMinorRevision, UINT theMajorRevision, UINT theVersion);
void getRegionVersion (UINT& theMinorRevision, UINT& theMajorRevision, UINT& theVersion) const;

    void addTcs (UINT theTcs, UINT theSubsystem);
    bool getTcs (size_t theIndex, UINT& theTcs, UINT& theSubsystem) const;
    size_t getNumTcs () const;
    void clearTcs ();

/**
   * Method setLastUpdateTime is called to set the update time of the
     * system operating parameters. 
     * theUpdateTime is the time at which the system configuration was 
     * last updated. 
   */
    void setLastUpdateTime (SYSTEMTIME theUpdateTime);

/**
   * Method getLastUpdateTime is called to get the update time of the
     * region details parameters.  
     * theUpdateTime returns the time at which the region details was 
     * last updated. 
   */
    bool getLastUpdateTime (SYSTEMTIME& theUpdateTime) const;

/**
   * Method toString returns a string representation of the region details
     * parameters. 
   */
std::string toString ();

private:
    void init ();

}; // class CRegionDetailsPdo

#include "cpdoexample.cd"
