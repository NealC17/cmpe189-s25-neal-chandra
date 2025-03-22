
#include "log-normal-model.h"

#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/pointer.h"
#include "ns3/string.h"

#include <cmath>

namespace ns3
{


NS_OBJECT_ENSURE_REGISTERED(LogNormalModel);
LogNormalModel::LogNormalModel ()
  : m_exponent (3.0),           // default exponent value
    m_referenceDistance (1.0),  // default reference distance, e.g., 1 meter
    m_referenceLoss (46.6777)   // example default reference loss in dB
{
  m_variable = CreateObject<NormalRandomVariable> ();
  m_variable->SetAttribute ("Mean", DoubleValue (0.0));
  m_variable->SetAttribute ("Variance", DoubleValue (1.0)); // Adjust the variance as needed
}
TypeId
LogNormalModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LogNormalModel")
    .SetParent<PropagationLossModel> ()
    .SetGroupName ("Propagation")
    .AddConstructor<LogNormalModel> ()
    // Add the random variable attribute for shadowing
    .AddAttribute ("Variable",
                   "The random variable stream used for log-normal shadowing",
                   StringValue ("ns3::NormalRandomVariable[Mean=0|Variance=1]"),
                   MakePointerAccessor (&LogNormalModel::m_variable),
                   MakePointerChecker<RandomVariableStream> ())
    // Optionally add additional attributes for the path loss parameters
    .AddAttribute ("Exponent",
                   "Path loss exponent",
                   DoubleValue (3.0),
                   MakeDoubleAccessor (&LogNormalModel::m_exponent),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("ReferenceDistance",
                   "Reference distance (meters)",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&LogNormalModel::m_referenceDistance),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("ReferenceLoss",
                   "Path loss (dB) at the reference distance",
                   DoubleValue (46.6777),
                   MakeDoubleAccessor (&LogNormalModel::m_referenceLoss),
                   MakeDoubleChecker<double> ());
  return tid;
}




void
LogNormalModel::SetPathLossExponent(double n)
{
    m_exponent = n;
}

void
LogNormalModel::SetReference(double referenceDistance, double referenceLoss)
{
    m_referenceDistance = referenceDistance;
    m_referenceLoss = referenceLoss;
}

double
LogNormalModel::GetPathLossExponent() const
{
    return m_exponent;


}
double
LogNormalModel::DoCalcRxPower(double txPowerDbm,
                                      Ptr<MobilityModel> a,
                                      Ptr<MobilityModel> b) const
{

  double distance = a->GetDistanceFrom (b);

  double pathLossDb = m_referenceLoss + 10.0 * m_exponent * std::log10 (distance);

  double shadowingDb = m_variable->GetValue ();
  pathLossDb += shadowingDb;

  return  txPowerDbm - pathLossDb;
}


int64_t
LogNormalModel::DoAssignStreams(int64_t stream)
{
    return 0;
}
}
