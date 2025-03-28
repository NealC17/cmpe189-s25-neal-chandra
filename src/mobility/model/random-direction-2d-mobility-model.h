/*
 * Copyright (c) 2007 INRIA
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef RANDOM_DIRECTION_MOBILITY_MODEL_H
#define RANDOM_DIRECTION_MOBILITY_MODEL_H

#include "constant-velocity-helper.h"
#include "mobility-model.h"
#include "rectangle.h"

#include "ns3/event-id.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/random-variable-stream.h"

namespace ns3
{

/**
 * \ingroup mobility
 * \brief Random direction mobility model.
 *
 * The movement of objects is based on random directions: each object
 * pauses for a specific delay, chooses a random direction and speed and
 * then travels in the specific direction until it reaches one of
 * the boundaries of the model. When it reaches the boundary, it pauses,
 * and selects a new direction and speed.
 */
class RandomDirection2dMobilityModel : public MobilityModel
{
  public:
    /**
     * Register this type with the TypeId system.
     * \return the object TypeId
     */
    static TypeId GetTypeId();
    RandomDirection2dMobilityModel();
    ~RandomDirection2dMobilityModel() override;

  private:
    /**
     * Set a new direction and speed
     */
    void ResetDirectionAndSpeed();
    /**
     * Pause, cancel currently scheduled event, schedule end of pause event
     */
    void BeginPause();
    /**
     * Set new velocity and direction, and schedule next pause event
     * \param direction (radians)
     */
    void SetDirectionAndSpeed(double direction);
    /**
     * Sets a new random direction and calls SetDirectionAndSpeed
     */
    void DoInitializePrivate();
    void DoDispose() override;
    void DoInitialize() override;
    Vector DoGetPosition() const override;
    void DoSetPosition(const Vector& position) override;
    Vector DoGetVelocity() const override;
    int64_t DoAssignStreams(int64_t) override;

    Ptr<UniformRandomVariable> m_direction; //!< rv to control direction
    Rectangle m_bounds;                     //!< the 2D bounding area
    Ptr<RandomVariableStream> m_speed;      //!< a random variable to control speed
    Ptr<RandomVariableStream> m_pause;      //!< a random variable to control pause
    EventId m_event;                        //!< event ID of next scheduled event
    ConstantVelocityHelper m_helper;        //!< helper for velocity computations
};

} // namespace ns3

#endif /* RANDOM_DIRECTION_MOBILITY_MODEL_H */
