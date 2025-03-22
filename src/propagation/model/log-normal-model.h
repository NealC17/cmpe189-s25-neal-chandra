#ifndef LOG_NORMAL_MODEL_H
#define LOG_NORMAL_MODEL_H

#include "ns3/object.h"
#include "ns3/random-variable-stream.h"
#include "ns3/propagation-loss-model.h"


#include <unordered_map>

namespace ns3
{

class LogNormalModel: public PropagationLossModel
{
  public:
    static TypeId GetTypeId();
    LogNormalModel();

    LogNormalModel(const LogNormalModel&) = delete;
    LogNormalModel& operator=(const LogNormalModel&) = delete;

    void SetPathLossExponent(double n);
    double GetPathLossExponent() const;

    void SetReference(double referenceDistance, double referenceLoss);
    double m_exponent;          
    double m_referenceDistance; 
    double m_referenceLoss;     
  private:
    double DoCalcRxPower(double txPowerDbm,
                         Ptr<MobilityModel> a,
                         Ptr<MobilityModel> b) const override;

    int64_t DoAssignStreams(int64_t stream) override;

    static Ptr<PropagationLossModel> CreateDefaultReference();
    Ptr<RandomVariableStream> m_variable;


};

}

#endif
